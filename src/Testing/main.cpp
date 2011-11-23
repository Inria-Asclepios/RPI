
#include <itkImageToVTKImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImage.h>

#include <vtkBox.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
#include <vtkCamera.h>
#include <vtkClipPolyData.h>
#include <vtkCommand.h>
#include <vtkExtentTranslator.h>
#include <vtkExtractVOI.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkImageData.h>
#include <vtkImageDataStreamer.h>
#include <vtkImageResample.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkMarchingCubes.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageActor.h>

#include <stdexcept>

typedef itk::Image<unsigned char, 3> ImageType;

void ClipRange( int & value, const int & minValue, const int & maxValue ) {
    if ( value < minValue ) value = minValue;
    if ( value > maxValue ) value = maxValue;
}

const double IsoValue = 127; // Isosurface value
const double WindowValue = 256;    // Window / level for image slices.
const double LevelValue = 127.5;
const double DownSamplingValue = 0.25; // down sampling on each axis.


void printBounds( const double * bounds )
{
    for ( int i(0); i<6; ++i) {
        if ( i != 0 )
            std::cout << " , ";
        std::cout << bounds[i];
    }
}
void printExtents( const int * extents )
{
    for ( int i(0); i<6; ++i) {
        if ( i != 0 )
            std::cout << " , ";
        std::cout << extents[i];
    }
}

class ImageSlicePipeline : public vtkObject
{
public:
    vtkTypeMacro(ImageSlicePipeline,vtkObject);

    static ImageSlicePipeline * New();

    struct AXIS { enum E {YZ = 0, XZ = 1, XY = 2 }; };

    void BuildPipeline( const std::string & inputFilename, AXIS::E axis ) 
    {
        // Reader
        Reader = ( ReaderType::New() );
        Reader->SetFileName( inputFilename );
        Reader->SetUseStreaming(true);

        // ITK -> VTK Filter
        this->ImageToVtk = ( ImageToVTKFilterType::New() );
        ImageToVtk->SetInput(Reader->GetOutput());

        clipper = ( vtkSmartPointer<vtkExtractVOI>::New() );
        clipper->SetInput(ImageToVtk->GetOutput());

        rWin = vtkSmartPointer<vtkRenderWindow>::New();
        rWinIn = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        ren = vtkSmartPointer<vtkRenderer>::New();

        this->Axis = axis;

        this->WindowLevel =  vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
        this->WindowLevel->SetInput(clipper->GetOutput());
        this->WindowLevel->SetWindow(WindowValue);
        this->WindowLevel->SetLevel(LevelValue);

        this->ImageActor = vtkSmartPointer<vtkImageActor>::New();
        this->ImageActor->SetInput(this->WindowLevel->GetOutput());

        int extent[6];
        this->GetImageExtent(extent);
        this->ImageActor->SetDisplayExtent(extent);

        ren->AddActor(this->ImageActor);
        rWin->AddRenderer(ren);
        rWinIn->SetRenderWindow(rWin);

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

    vtkGetMacro( SliceNum, int );
    void SetSliceNum( int num ) {
        this->SliceNum = num;
        int extent[6];
        this->GetImageExtent(extent);
        this->ImageActor->SetDisplayExtent(extent);
        ren->ResetCamera();
        rWin->Render();
        rWinIn->Render();
    }

    void SetFullExtent( const int * extentFull ) {
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

    vtkImageActor * getActor() { return this->ImageActor; }

protected:
    ImageSlicePipeline() : SliceNum(0) {}

    void GetImageExtent( int extent[6] )
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
    AXIS::E Axis;

    int SliceNum;
    vtkSmartPointer<vtkImageMapToWindowLevelColors> WindowLevel;
    vtkSmartPointer<vtkImageActor> ImageActor;

    vtkSmartPointer<vtkRenderWindow> rWin;
    vtkSmartPointer<vtkRenderWindowInteractor> rWinIn;
    vtkSmartPointer<vtkRenderer> ren;

    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer Reader;

    // ITK -> VTK Filter
    typedef itk::ImageToVTKImageFilter<ImageType> ImageToVTKFilterType;
    ImageToVTKFilterType::Pointer ImageToVtk;

    vtkSmartPointer<vtkExtractVOI> clipper;

};

vtkStandardNewMacro(ImageSlicePipeline);

// Callback for the interaction
class vtkBWCallback2 : public vtkCommand
{
public:
    static vtkBWCallback2 *New() 
    { return new vtkBWCallback2; }

    virtual void Execute(vtkObject *caller, unsigned long evId, void*)
    {
        if ( evId != vtkCommand::TimerEvent) {
            vtkBoxWidget2 *boxWidget = vtkBoxWidget2::SafeDownCast(caller);

            vtkBoxRepresentation *boxRep = vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation());

            //boxRep->GetTransform(this->Transform);

            //this->ClipBox->SetTransform(this->Transform);

            double bounds[6];
            const double * brBounds = boxRep->GetBounds();
            std::copy( brBounds, brBounds+6, bounds);
            {
                //printBounds(bounds);   std::cout << std::endl;
                double origin[3], spacing[3];
                int extent[6];
                vtkImageData * id = vtkImageData::SafeDownCast( this->ExtractVOI->GetInput());
                id->UpdateInformation();
                id->GetSpacing(spacing);
                id->GetOrigin(origin);
                id->GetWholeExtent(extent);
                int range[6];
                for ( int i(0); i<3; ++i) {
                    range[2*i] = std::floor( (bounds[2*i] - origin[i]) / spacing[i] );
                    range[2*i + 1] = std::ceil( (bounds[2*i + 1] - origin[i]) / spacing[i] );

                    range[2*i] = std::max( range[2*i], extent[2*i] );
                    range[2*i + 1] = std::min( range[2*i + 1], extent[2*i + 1] );
                }

                std::copy( range, range+6, LatestExtent);
                std::copy( bounds, bounds+6, LatestBounds);
            }

            double centreBound[3];
            for ( int i(0); i<3; ++i) {
                centreBound[i] = bounds[2*i] + ( ( bounds[2*i + 1] - bounds[2*i]) *0.5 );
            }

            this->AxisActorXY->SetFullExtent( LatestExtent );
            this->AxisActorXZ->SetFullExtent( LatestExtent );
            this->AxisActorYZ->SetFullExtent( LatestExtent );

            //printExtents(extent);   std::cout << std::endl;
            //printExtents(range);   std::cout << std::endl;

            boxWidget->GetInteractor()->Render();

            if ( RenWin )
                RenWin->ResetTimer(this->TimerId);
            else {
                this->ExtractVOI->SetVOI(LatestExtent);
                this->ClipBox->SetBounds(LatestBounds);
            }
        } else {

            std::cout << "Current slice extent : iMin,iMax, jMin,jMax, kMin,kMax"<<std::endl;
            printExtents(LatestExtent); std::cout << std::endl;

            this->ExtractVOI->SetVOI(LatestExtent);
            this->ClipBox->SetBounds(LatestBounds);
            if ( RenWin )
                RenWin->Render();
        }
    }

    vtkBWCallback2() {
        this->Transform = vtkSmartPointer<vtkTransform>::New();
        this->TimerId = -1;
    }

    ~vtkBWCallback2() {
        if ( this->TimerId != -1 )
            RenWin->DestroyTimer(this->TimerId);
    }

    void SetClipBox( vtkBox * box ) { this->ClipBox = box; }
    void SetVOIExtractor( vtkExtractVOI * Extractor) { this->ExtractVOI = Extractor; }
    void SetRenderWindowInteractor( vtkRenderWindowInteractor * iren) {
        this->RenWin = iren;
        this->TimerId = this->RenWin->CreateOneShotTimer(500);
        this->RenWin->AddObserver(vtkCommand::TimerEvent, this);
    }

    void SetSliceActors( ImageSlicePipeline * xy, ImageSlicePipeline * xz, ImageSlicePipeline * yz ){
        this->AxisActorXY = xy;
        this->AxisActorXZ = xz;
        this->AxisActorYZ = yz;
    }

protected:
    vtkSmartPointer<vtkBox> ClipBox;
    vtkSmartPointer<vtkTransform> Transform;
    vtkSmartPointer<vtkExtractVOI> ExtractVOI;
    vtkSmartPointer<vtkRenderWindowInteractor> RenWin;

    vtkSmartPointer<ImageSlicePipeline> AxisActorXY;
    vtkSmartPointer<ImageSlicePipeline> AxisActorXZ;
    vtkSmartPointer<ImageSlicePipeline> AxisActorYZ;

    int LatestExtent[6];
    double LatestBounds[6];
    int TimerId;
};

vtkSmartPointer<vtkBoxWidget2> addBoxWidget(const double * bounds, vtkRenderWindowInteractor * iren, vtkBWCallback2 * myCallback )
{
    double boundsLocal[6];
    std::copy( bounds, bounds+6, boundsLocal);
    vtkSmartPointer<vtkBoxRepresentation> boxRep = vtkSmartPointer<vtkBoxRepresentation>::New();
    boxRep->SetPlaceFactor( 0.2 );
    boxRep->PlaceWidget(boundsLocal);

    vtkSmartPointer<vtkBoxWidget2> boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
    boxWidget->SetInteractor( iren );
    boxWidget->SetRepresentation( boxRep );
    boxWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
    boxWidget->SetPriority(1);
    boxWidget->RotationEnabledOff();

    return boxWidget;
}

class LowResPipeline : public vtkObject
{
public:
    vtkTypeMacro(LowResPipeline,vtkObject);
    
    static LowResPipeline * New();

    void BuildPipeLine(const std::string & inputFilename, vtkBox * clipBoxIn) {
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

    vtkActor * getActor() { return surfaceActor; }
    vtkImageData * GetDownSampledImage() { return this->DownsampledImageVtk; }

    void GetImageBounds( double * bounds ) {
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
protected:
    LowResPipeline() {}

    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer Reader;

    // ITK -> VTK Filter
    typedef itk::ImageToVTKImageFilter<ImageType> ImageToVTKFilterType;
    ImageToVTKFilterType::Pointer ImageToVtk;

    typedef vtkSmartPointer<vtkImageResample> ResamplerPtrType;
    ResamplerPtrType Resampler;

    typedef vtkSmartPointer<vtkImageDataStreamer> ImageDataStreamerPtrType;
    ImageDataStreamerPtrType ImageStreamer;

    typedef vtkSmartPointer<vtkImageData> ImageDataPtrType;
    ImageDataPtrType DownsampledImageVtk;


    vtkSmartPointer<vtkMarchingCubes> surfaceGen;

    vtkSmartPointer<vtkBox> clipBox;

    vtkSmartPointer<vtkExtractPolyDataGeometry> clipper;


    vtkSmartPointer<vtkActor> surfaceActor;
    vtkSmartPointer<vtkPolyDataMapper> surfaceMapper;
};

vtkStandardNewMacro(LowResPipeline);


class HighResPipeline : public vtkObject
{
public:
    vtkTypeMacro(HighResPipeline,vtkObject);

    static HighResPipeline * New();

    void BuildPipeLine(const std::string & inputFilename) {
        // Reader
        Reader = ( ReaderType::New() );
        Reader->SetFileName( inputFilename );
        Reader->SetUseStreaming(true);

        // ITK -> VTK Filter
        this->ImageToVtk = ( ImageToVTKFilterType::New() );
        ImageToVtk->SetInput(Reader->GetOutput());

        clipper = ( vtkSmartPointer<vtkExtractVOI>::New() );
        clipper->SetInput(ImageToVtk->GetOutput());

        surfaceGen = vtkSmartPointer<vtkMarchingCubes>::New();
        surfaceGen->SetInput(clipper->GetOutput());
        surfaceGen->SetNumberOfContours(1);
        surfaceGen->SetValue(0, IsoValue);

        surfaceActor = vtkSmartPointer<vtkActor>::New();
        surfaceMapper =vtkSmartPointer<vtkPolyDataMapper>::New();
        surfaceMapper->SetInput(surfaceGen->GetOutput());
        surfaceMapper->SetScalarVisibility(0);

        surfaceActor->GetProperty()->SetColor(1,0,0);
        surfaceActor->SetMapper(surfaceMapper);

    }

    vtkActor * getActor() { return surfaceActor; }
    vtkExtractVOI * GetExtractVOI() { return this->clipper; }
protected:
    HighResPipeline() {}

    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer Reader;

    // ITK -> VTK Filter
    typedef itk::ImageToVTKImageFilter<ImageType> ImageToVTKFilterType;
    ImageToVTKFilterType::Pointer ImageToVtk;

    vtkSmartPointer<vtkMarchingCubes> surfaceGen;

    vtkSmartPointer<vtkExtractVOI> clipper;

    vtkSmartPointer<vtkActor> surfaceActor;
    vtkSmartPointer<vtkPolyDataMapper> surfaceMapper;
};

vtkStandardNewMacro(HighResPipeline);

int main(int argc, char * argv[])
{

    //const std::string inputFilename = "c:/Temp/brebix.mha";
    //const std::string inputFilename = "c:/Temp/testfile.mha";
    //const std::string inputFilename = "C:/Users/jstark/Projects/sampleData/AMC_2011-11/Ex365Em505T12000ms.nii";
    if (argc != 2 ) {
        std::cerr << "Usage " << argv[0] << " filename" <<std::endl;
        return 1;
    }
    const std::string inputFilename = argv[1]; //"C:/Users/jstark/Projects/sampleData/AMC_2011-11/pig in ii.nii";

    vtkSmartPointer<vtkBox> clipBox = vtkSmartPointer<vtkBox>::New();

    vtkSmartPointer<LowResPipeline> lowRes = vtkSmartPointer<LowResPipeline>::New();
    lowRes->BuildPipeLine(inputFilename, clipBox);

    vtkSmartPointer<HighResPipeline> highRes = vtkSmartPointer<HighResPipeline>::New();
    highRes->BuildPipeLine(inputFilename);

    vtkSmartPointer<ImageSlicePipeline> SliceXY = vtkSmartPointer<ImageSlicePipeline>::New();
    vtkSmartPointer<ImageSlicePipeline> SliceYZ = vtkSmartPointer<ImageSlicePipeline>::New();
    vtkSmartPointer<ImageSlicePipeline> SliceXZ = vtkSmartPointer<ImageSlicePipeline>::New();

    SliceXY->BuildPipeline(inputFilename, ImageSlicePipeline::AXIS::XY);
    SliceXZ->BuildPipeline(inputFilename, ImageSlicePipeline::AXIS::XZ);
    SliceYZ->BuildPipeline(inputFilename, ImageSlicePipeline::AXIS::YZ);

    vtkSmartPointer<vtkBWCallback2> myCallback = vtkSmartPointer<vtkBWCallback2>::New();
    myCallback->SetSliceActors(SliceXY, SliceXZ, SliceYZ);
    myCallback->SetClipBox(clipBox);
    myCallback->SetVOIExtractor(highRes->GetExtractVOI());

    // Renderer and window
    vtkSmartPointer<vtkRenderWindow> rWin = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> rWinIn = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();

    ren->AddActor(lowRes->getActor());
    ren->AddActor(highRes->getActor());

    double bounds[6];
    lowRes->GetImageBounds(bounds);
    if ( bounds[1] - bounds[0] <= 0 ) {
        throw( std::runtime_error("Bad bounds") );
    }

    //printBounds(bounds);   std::cout << std::endl;

    vtkSmartPointer<vtkBoxWidget2> boxWidget = addBoxWidget(bounds, rWinIn, myCallback);

    rWin->AddRenderer(ren);
    rWinIn->SetRenderWindow(rWin);
    rWin->SetSize(800, 640);
    vtkInteractorStyleSwitch * sw = vtkInteractorStyleSwitch::SafeDownCast( rWinIn->GetInteractorStyle() );
    if ( sw ) 
        sw->SetCurrentStyleToTrackballCamera();

    boxWidget->EnabledOn();

    //Force initial update
    myCallback->Execute(boxWidget,0,0);

    rWin->Render();
    myCallback->SetRenderWindowInteractor(rWinIn);
    vtkBoxRepresentation *boxRep = vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation());
    clipBox->SetBounds(boxRep->GetBounds());

    ren->ResetCamera();

    rWinIn->Render();
    rWinIn->Start();
    
    return 0;
}
