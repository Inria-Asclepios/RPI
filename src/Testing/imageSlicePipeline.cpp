
#include "imageSlicePipeline.h"

vtkStandardNewMacro(ImageSlicePipeline);

namespace {
    void ClipRange( int & value, const int & minValue, const int & maxValue ) {
        if ( value < minValue ) value = minValue;
        if ( value > maxValue ) value = maxValue;
    }
}

void ImageSlicePipeline::BuildPipeline( const std::string & inputFilename, AXIS::E axis, vtkRenderWindow * rWinArg )
{
    this->rWin = rWinArg;

    // Reader
    Reader = ( ReaderType::New() );
    Reader->SetFileName( inputFilename );
    Reader->SetUseStreaming(true);

    // ITK -> VTK Filter
    this->ImageToVtk = ( ImageToVTKFilterType::New() );
    ImageToVtk->SetInput(Reader->GetOutput());

    clipper = ( vtkSmartPointer<vtkExtractVOI>::New() );
    clipper->SetInput(ImageToVtk->GetOutput());

    ren = vtkSmartPointer<vtkRenderer>::New();

    this->Axis = axis;

    this->WindowLevel->SetInput(clipper->GetOutput());

    this->ImageActor = vtkSmartPointer<vtkImageActor>::New();
    this->ImageActor->SetInput(this->WindowLevel->GetOutput());

    int extent[6];
    this->GetImageExtent(extent);
    this->ImageActor->SetDisplayExtent(extent);

    ren->AddActor(this->ImageActor);
    rWin->AddRenderer(ren);

    double camFp[3] = {0,0,0};
    double camPos[3] = {0,0,0};
    camPos[axis] = 1;
    double camUp[3] = {0,0,0};
    camUp[(axis+2)%3] = 1;

    ren->GetActiveCamera()->SetFocalPoint(camFp);
    ren->GetActiveCamera()->SetPosition(camPos);
    ren->GetActiveCamera()->SetViewUp(camUp);
    ren->GetActiveCamera()->SetParallelProjection(1);

    ren->ResetCamera();
}

void ImageSlicePipeline::SetSliceNum( int num )
{
    this->SliceNum = num;
    int extent[6];
    this->GetImageExtent(extent);
    this->ImageActor->SetDisplayExtent(extent);
    ren->ResetCamera();
    rWin->Render();
}

void ImageSlicePipeline::SetFullExtent( const int * extentFull )
{
    int axis = 0;
    switch( this->Axis ) {
    case AXIS::XY: 
        axis = 2;
        break;
    case AXIS::XZ: 
        axis = 1;
        break;
    default:
    case AXIS::YZ: 
        axis = 0;
        break;
    }
    int maxExtent[6];
    this->ImageToVtk->GetOutput()->UpdateInformation();
    this->ImageToVtk->GetOutput()->GetWholeExtent(maxExtent);

    int extentCopy[6];
    std::copy( extentFull, extentFull + 6, extentCopy);

    for ( int i(0); i<3; ++i ) {
        ClipRange(extentCopy[2*i], extentFull[2*i], extentFull[2*i +1] );
        ClipRange(extentCopy[2*i + 1], extentFull[2*i], extentFull[2*i +1] );
    }
    clipper->SetVOI(extentCopy);
    int sliceNum = extentFull[2*axis] + ( (extentFull[2*axis +1] - extentFull[2*axis] ) / 2 );
    this->SetSliceNum(sliceNum);
}

void ImageSlicePipeline::GetImageExtent( int extent[6] )
{
    vtkImageData * input = clipper->GetOutput();
    input->UpdateInformation();

    input->GetWholeExtent(extent);
    switch( this->Axis ) {
    case AXIS::XY: 
        extent[4] = extent[5] = this->GetSliceNum();
        break;
    case AXIS::XZ: 
        extent[2] = extent[3] = this->GetSliceNum();
        break;
    default:
    case AXIS::YZ: 
        extent[0] = extent[1] = this->GetSliceNum();
        break;
    }
}

void ImageSlicePipeline::SetWindowLevel( double w, double l )
{
    this->WindowLevel->SetWindow(w);
    this->WindowLevel->SetLevel(l);
}

ImageSlicePipeline::ImageSlicePipeline() : SliceNum(0)
{
    this->WindowLevel =  vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
}
