
#include <itkImageToVTKImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImage.h>

#include <vtkBox.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
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

#include <stdexcept>

typedef itk::Image<unsigned short, 3> ImageType;



const double IsoValue = 1000;

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
protected:
    vtkSmartPointer<vtkBox> ClipBox;
    vtkSmartPointer<vtkTransform> Transform;
    vtkSmartPointer<vtkExtractVOI> ExtractVOI;
    vtkSmartPointer<vtkRenderWindowInteractor> RenWin;

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
            Resampler->SetAxisMagnificationFactor(axis,0.25);
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

    void GetImageBounds( double * bounds ) {
        DownsampledImageVtk->Update();
        double range[2];
        DownsampledImageVtk->GetScalarRange(range);
        std::cout << " Range : " << range[0] << " to " << range[1] << std::endl;
        return DownsampledImageVtk->GetBounds(bounds);
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
    const std::string inputFilename = "C:/Users/jstark/Projects/sampleData/AMC_2011-11/Ex365Em505T12000ms.nii";
    vtkSmartPointer<vtkBox> clipBox = vtkSmartPointer<vtkBox>::New();

    vtkSmartPointer<LowResPipeline> lowRes = vtkSmartPointer<LowResPipeline>::New();
    lowRes->BuildPipeLine(inputFilename, clipBox);

    vtkSmartPointer<HighResPipeline> highRes = vtkSmartPointer<HighResPipeline>::New();
    highRes->BuildPipeLine(inputFilename);

    vtkSmartPointer<vtkBWCallback2> myCallback = vtkSmartPointer<vtkBWCallback2>::New();
    myCallback->SetClipBox(clipBox);
    myCallback->SetVOIExtractor(highRes->GetExtractVOI());

    // Renderer and window
    vtkSmartPointer<vtkRenderWindow> rWin = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> rWinIn = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();

    ren->AddActor(lowRes->getActor());
    ren->AddActor(highRes->getActor());

    rWin->AddRenderer(ren);
  
    double bounds[6];
    lowRes->GetImageBounds(bounds);
    if ( bounds[1] - bounds[0] <= 0 ) {
        throw( std::runtime_error("Bad bounds") );
    }

    //printBounds(bounds);   std::cout << std::endl;

    vtkSmartPointer<vtkBoxWidget2> boxWidget = addBoxWidget(bounds, rWinIn, myCallback);

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
