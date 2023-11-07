#ifndef _RPI_TREX_HXX_
#define _RPI_TREX_HXX_


#include <itkEuler3DTransform.h>
#include "rpiRegistrationMethod.hxx"


// Namespace RPI : Registration Programming Interface
namespace rpi
{


/**
 * TRex (Toy Registration EXample) regstration method.
 * This method is a fully ITK registration method implemented into the RPI framework.
 * The method is simple and gives a clear example of how a registration method of RPI should be implemented.
 *
 * There are three templates for this class:
 *
 *   TFixedImage           Type of the fixed image. Must be an itk::Image< TPixel, 3 > where
 *                         TPixel is a scalar type (e.g. unsigned char, short, float, etc.).
 *
 *   TMovingImage          Type of the moving image. Must be an itk::Image< TPixel, 3 > where
 *                         TPixel is a scalar type (e.g. unsigned char, short, float, etc.).
 *
 *   TTransformScalarType  Type of the transformation parameters. Must be "double".
 *
 * @author Vincent Garcia
 * @date   2011/03/15
 */
template < class TFixedImage, class TMovingImage, class TTransformScalarType=double >
class ITK_EXPORT TRex : public RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >{

public:

    typedef itk::Euler3DTransform<TTransformScalarType>
            TransformType;

    typedef typename TransformType::Pointer
            TransformPointerType;


protected:

    /**
     * Maximal number of iterations.
     */
    unsigned int      m_iterations;


public:


    /**
     * Class constructor.
     */
    TRex(void);


    /**
     * Class destructor.
     */
    virtual ~TRex(void);


    /**
     * Returns thenumber of iterations allowed for the registration process.
     * @return  number of iterations
     */
    unsigned int      GetNumberOfIterations(void) const;


    /**
     * Sets the number of iterations allowed for the registration process.
     * This number must be greater than or equal to the number of interpolations.
     * @param  value  number of iterations
     */
    void              SetNumberOfIterations(unsigned int value);


    /**
     * Performs the image registration.
     */
    virtual void      StartRegistration(void);

};


} // End of namespace


/** Add the source code file (template) */
#include "rpiTRex.cxx"


#endif // _RPI_TREX_HXX_
