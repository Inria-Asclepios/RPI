#include "highResPipeline.h"

vtkStandardNewMacro(HighResPipeline);

void HighResPipeline::BuildPipeLine( const std::string & inputFilename )
{
    // Reader
    Reader = ( ReaderType::New() );
    Reader->SetFileName( inputFilename );
    Reader->SetUseStreaming(true);

    // ITK -> VTK Filter
    this->ImageToVtk = ( ImageToVTKFilterType::New() );
    ImageToVtk->SetInput(Reader->GetOutput());

    int initialVOI[6] = {0,0,0,0,0,0};
    clipper = ( vtkSmartPointer<vtkExtractVOI>::New() );
    clipper->SetVOI(initialVOI);
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
