#include "lowResPipeline.h"

vtkStandardNewMacro(LowResPipeline);

void LowResPipeline::GetImageBounds( double * bounds )
{
    DownsampledImageVtk->Update();
    double range[2];
    DownsampledImageVtk->GetScalarRange(range);
    std::cout << " Range : " << range[0] << " to " << range[1] << std::endl;

    double spacing[3];
    double origin[3];
    DownsampledImageVtk->GetOrigin(origin);
    DownsampledImageVtk->GetSpacing(spacing);
    int extent[6];
    DownsampledImageVtk->GetWholeExtent(extent);
    for(int i(0); i<3; ++i) {
        bounds[2*i] = origin[i] + extent[2*i]*spacing[i];
        bounds[2*i + 1] = origin[i] + extent[2*i+1]*spacing[i];
    }
    return;
}

void LowResPipeline::BuildPipeLine( const std::string & inputFilename, vtkBox * clipBoxIn, double DownSamplingValue )
{
    // Reader
    Reader = ( ReaderType::New() );
    Reader->SetFileName( inputFilename );
    Reader->SetUseStreaming(true);

    // ITK -> VTK Filter
    this->ImageToVtk = ( ImageToVTKFilterType::New() );
    ImageToVtk->SetInput(Reader->GetOutput());

    this->Resampler = ( ResamplerPtrType::New() );
    Resampler->SetInput(ImageToVtk->GetOutput());
    for (int axis(0); axis<3; ++axis) {
        Resampler->SetAxisMagnificationFactor(axis,DownSamplingValue);
    }

    ImageStreamer = ( ImageDataStreamerPtrType::New() );
    ImageStreamer->SetInput(Resampler->GetOutput());
    ImageStreamer->UpdateInformation();
    // We could dynamically determine the number of divisions.
    ImageStreamer->SetNumberOfStreamDivisions(8);
    ImageStreamer->GetExtentTranslator()->SetSplitModeToBlock();

    DownsampledImageVtk = ImageStreamer->GetOutput();


    surfaceGen = vtkSmartPointer<vtkMarchingCubes>::New();
    surfaceGen->SetInput(ImageStreamer->GetOutput());
    surfaceGen->SetNumberOfContours(1);
    surfaceGen->SetValue(0, IsoValue);

    clipBox = clipBoxIn;

    clipper = ( vtkSmartPointer<vtkExtractPolyDataGeometry>::New() );
    clipper->SetInput(surfaceGen->GetOutput());
    //clipper->SetClipFunction(clipBox);
    clipper->SetImplicitFunction(clipBox);
    //clipper->SetInsideOut(0);
    clipper->SetExtractInside(0);

    surfaceActor = vtkSmartPointer<vtkActor>::New();
    surfaceMapper =vtkSmartPointer<vtkPolyDataMapper>::New();
    surfaceMapper->SetInput(clipper->GetOutput());
    surfaceMapper->SetScalarVisibility(0);

    surfaceActor->GetProperty()->SetColor(0,0,1);
    surfaceActor->SetMapper(surfaceMapper);
}
