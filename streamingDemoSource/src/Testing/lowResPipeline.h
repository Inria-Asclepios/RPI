#ifndef LOWRESPIPELINE_H
#define LOWRESPIPELINE_H

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

class LowResPipeline : public PipelineCommon
{
public:
    vtkTypeMacro(LowResPipeline,PipelineCommon);

    static LowResPipeline * New();

    void BuildPipeLine(const std::string & inputFilename, vtkBox * clipBoxIn, double DownSamplingFactor);

    vtkActor * getActor() { return surfaceActor; }
    vtkImageData * GetDownSampledImage() { return this->DownsampledImageVtk; }

    void GetImageBounds( double * bounds );
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



#endif // LOWRESPIPELINE_H
