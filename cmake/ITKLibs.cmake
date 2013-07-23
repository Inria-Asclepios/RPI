# This defines useful cmake variables for ITK libs

set(ITKIO_LIBRARIES
  ITKIOBMP
  ITKIOBioRad
  ITKIOHDF5
  ITKIOGDCM
  ITKIOGIPL
  ITKIOJPEG
  ITKIOLSM
  ITKIOMeta
  ITKIONIFTI
  ITKIONRRD
  ITKIOPNG
  ITKIOStimulate
  ITKIOVTK
)

set(ITK_TRANSFORM_LIBRARIES
  ITKIOTransformBase
  ITKIOTransformInsightLegacy
  ITKIOTransformHDF5
  ITKIOTransformMatlab
)