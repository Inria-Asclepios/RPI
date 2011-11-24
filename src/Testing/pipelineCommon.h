#ifndef PIPELINECOMMON_H
#define PIPELINECOMMON_H

#include <itkImage.h>
#include <vtkObject.h>

typedef itk::Image<unsigned char, 3> ImageType;

class PipelineCommon : public vtkObject
{
public:
    vtkTypeMacro(PipelineCommon,vtkObject);
    vtkSetMacro(IsoValue, double);
    vtkGetMacro(IsoValue, double);
protected:
    PipelineCommon();
    double IsoValue;
};

#endif // PIPELINECOMMON_H
