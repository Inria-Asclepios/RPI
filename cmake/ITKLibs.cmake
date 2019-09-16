# This defines useful cmake variables for ITK libs

set(ITKIO_LIBRARIES
    ITKIOImageBase
    ITKIOBMP
    ITKIOBioRad
    ITKIOHDF5
    ITKIOGDCM
    ITKIOGE
    ITKIOGIPL
    ITKIOJPEG
    ITKIOJPEG2000
    ITKIOLSM
    ITKIOMeta
    ITKIONIFTI
    ITKIONRRD
    ITKIOPNG
    ITKIOStimulate
    ITKIOVTK
    ITKIOMRC
    ITKIOTIFF
    ITKIOBruker
    ITKIOMINC
	${ITKIOPhilipsREC_LIBRARIES}
)

set(ITK_TRANSFORM_LIBRARIES
    ITKIOTransformBase
    ITKIOTransformInsightLegacy
    ITKIOTransformHDF5
    ITKIOTransformMatlab
)