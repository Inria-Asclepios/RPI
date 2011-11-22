
#include <itkImageToVTKImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImage.h>

#include <vtkExtentTranslator.h>
#include <vtkImageData.h>
#include <vtkImageDataStreamer.h>
#include <vtkImageResample.h>
#include <vtkMarchingCubes.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>

typedef itk::Image<short, 3> ImageType;

int main(int argc, char * argv[])
{

    const std::string inputFilename = "c:/Temp/brebix.mha";
    // Reader
    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer reader( ReaderType::New() );
    reader->SetFileName( inputFilename );
    reader->SetUseStreaming(true);

    // ITK -> VTK Filter
    typedef itk::ImageToVTKImageFilter<ImageType> ImageToVTKFilterType;
    ImageToVTKFilterType::Pointer imageToVtk( ImageToVTKFilterType::New() );
    imageToVtk->SetInput(reader->GetOutput());

    typedef vtkSmartPointer<vtkImageResample> ResamplerPtrType;
    ResamplerPtrType resampler( ResamplerPtrType::New() );
    resampler->SetInput(imageToVtk->GetOutput());
    for (int axis(0); axis<3; ++axis) {
        resampler->SetAxisMagnificationFactor(axis,0.25);
    }

    typedef vtkSmartPointer<vtkImageDataStreamer> ImageDataStreamerPtrType;
    ImageDataStreamerPtrType imageStreamer( ImageDataStreamerPtrType::New() );
    imageStreamer->SetInput(resampler->GetOutput());
    imageStreamer->UpdateInformation();
    // We could dynamically determine the number of divisions.
    imageStreamer->SetNumberOfStreamDivisions(8);
    imageStreamer->GetExtentTranslator()->SetSplitModeToBlock();

    typedef vtkSmartPointer<vtkImageData> ImageDataPtrType;
    ImageDataPtrType downsampledImageVtk = imageStreamer->GetOutput();

    
    vtkSmartPointer<vtkMarchingCubes> surfaceGen = vtkSmartPointer<vtkMarchingCubes>::New();
    surfaceGen->SetInput(imageStreamer->GetOutput());
    surfaceGen->SetNumberOfContours(1);
    surfaceGen->SetValue(0, 200);

    vtkSmartPointer<vtkActor> surfaceActor = vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkPolyDataMapper> surfaceMapper =vtkSmartPointer<vtkPolyDataMapper>::New();
    surfaceMapper->SetInput(surfaceGen->GetOutput());

    surfaceActor->SetMapper(surfaceMapper);

    vtkSmartPointer<vtkRenderWindow> rWin = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> rWinIn = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();

    ren->AddActor(surfaceActor);

    rWin->AddRenderer(ren);
    
    rWinIn->SetRenderWindow(rWin);
    rWin->SetSize(800, 640);
    rWin->Render();

    rWinIn->Render();
    rWinIn->Start();
    
    return 0;
}
