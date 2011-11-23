#ifndef IMAGESLICEPIPELINE_H
#define IMAGESLICEPIPELINE_H
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

class ImageSlicePipeline : public vtkObject
{
public:
    vtkTypeMacro(ImageSlicePipeline,vtkObject);

    static ImageSlicePipeline * New();

    struct AXIS { enum E {YZ = 0, XZ = 1, XY = 2 }; };

    void BuildPipeline( const std::string & inputFilename, AXIS::E axis, vtkRenderWindow * rWin );

    vtkGetMacro( SliceNum, int );
    void SetSliceNum( int num );

    void SetFullExtent( const int * extentFull );

    vtkImageActor * getActor() { return this->ImageActor; }

    void SetWindowLevel( double w, double l);

protected:
    ImageSlicePipeline();

    void GetImageExtent( int extent[6] );
    AXIS::E Axis;

    int SliceNum;
    vtkSmartPointer<vtkImageMapToWindowLevelColors> WindowLevel;
    vtkSmartPointer<vtkImageActor> ImageActor;

    vtkSmartPointer<vtkRenderWindow> rWin;
    vtkSmartPointer<vtkRenderer> ren;

    typedef itk::ImageFileReader<ImageType> ReaderType;
    ReaderType::Pointer Reader;

    // ITK -> VTK Filter
    typedef itk::ImageToVTKImageFilter<ImageType> ImageToVTKFilterType;
    ImageToVTKFilterType::Pointer ImageToVtk;

    vtkSmartPointer<vtkExtractVOI> clipper;
};
#endif // IMAGESLICEPIPELINE_H
