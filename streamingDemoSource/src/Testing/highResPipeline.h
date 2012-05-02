#ifndef HIGHRESPIPELINE_H
#define HIGHRESPIPELINE_H

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

#include "pipelineCommon.h"

class HighResPipeline : public PipelineCommon
{
public:
    vtkTypeMacro(HighResPipeline,vtkObject);

    static HighResPipeline * New();

    void BuildPipeLine(const std::string & inputFilename);

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


#endif // HIGHRESPIPELINE_H
