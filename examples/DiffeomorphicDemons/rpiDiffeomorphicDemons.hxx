#ifndef _RPI_DIFFEOMORPHIC_DEMONS_HXX_
#define _RPI_DIFFEOMORPHIC_DEMONS_HXX_



#include <itkDisplacementFieldTransform.h>
#include "rpiRegistrationMethod.hxx"


// Namespace RPI : Registration Programming Interface
namespace rpi
{


/**
 * Diffeomorphic demons registration method. This class is based on the ITK diffeormorphic demons
 * implementation (filter).
 *
 * There are three templates for this class:
 *
 *   TFixedImage           Type of the fixed image. Must be an itk::Image< TPixel, 3 > where
 *                         TPixel is a scalar type among "float" and "double".
 *
 *   TMovingImage          Type of the moving image. Must be an itk::Image< TPixel, 3 > where
 *                         TPixel is a scalar type among "float" and "double".
 *
 *   TTransformScalarType  Type of the transformation parameters. Must be "float" or "double".
 *
 * In the current implementation, TFixedImage and TMovingImage must be identical.
 *
 * @author Vincent Garcia and Tom Vercauteren
 * @date   2010/10/29
 */
template < class TFixedImage, class TMovingImage, class TTransformScalarType=double >
class ITK_EXPORT DiffeomorphicDemons : public RegistrationMethod< TFixedImage, TMovingImage, TTransformScalarType >{


public:

    /**
     * Update rule
     */
    enum UpdateRule {
        UPDATE_DIFFEOMORPHIC, /** s <- s o exp(u)                       */
        UPDATE_ADDITIVE,      /** s <- s + u      (ITK basic)           */
        UPDATE_COMPOSITIVE    /** s <- s o (Id+u) (Thirion's proposal?) */
    };

    /**
     * Gradient type
     */
    enum GradientType {
        GRADIENT_SYMMETRIZED,         /** symmetrized         */
        GRADIENT_FIXED_IMAGE,         /** fixed image         */
        GRADIENT_WARPED_MOVING_IMAGE, /** warped moving image */
        GRADIENT_MAPPED_MOVING_IMAGE  /** mapped moving image */
    };

    typedef itk::DisplacementFieldTransform< TTransformScalarType, TFixedImage::ImageDimension >
            TransformType;

    typedef typename TransformType::Pointer
            TransformPointerType;


protected:

    /**
     * Number of iterations per level of resolution (from coarse to fine levels).
     */
    std::vector<unsigned int>   m_iterations;


    /**
     * Update rule.
     */
    UpdateRule                  m_updateRule;


    /**
     * Type of gradient used for computing the demons force.
     */
    GradientType                m_gradientType;


    /**
     * Maximum length of an update vector (voxel units).
     */
    float                       m_maximumUpdateStepLength;


    /**
     * Standard deviation (in voxel unit) of the update field (Gaussian) smoothing.
     */
    float                       m_updateFieldStandardDeviation;


    /**
     * Standard deviation (in voxel unit) of the displacement field (Gaussian) smoothing.
     */
    float                       m_displacementFieldStandardDeviation;


    /**
     * Initial transformation.
     */
    TransformPointerType        m_initialTransform;


    /**
     * Histogram matching
     */
    bool                        m_useHistogramMatching;


public:

    /**
     * Class constructor.
     */
    DiffeomorphicDemons(void);


    /**
     * Class destructor.
     */
    virtual ~DiffeomorphicDemons(void);


    /**
     * Gets the number of iterations per level of resolution.
     * @return  number of iterations
     */
    std::vector<unsigned int>   GetNumberOfIterations(void) const;


    /**
     * Sets the number of iterations per level of resolution.
     * @param  iterations  number of iterations
     */
    void                        SetNumberOfIterations(std::vector<unsigned int> iterations);


    /**
     * Gets the update rule.
     * @return  update rule
     */
    UpdateRule                  GetUpdateRule(void) const;


    /**
     * Gets the update rule as string.
     * @return  string describing the update rule
     */
    std::string                 GetUpdateRuleAsString(void) const;


    /**
     * Sets the update rule:
     *   UPDATE_DIFFEOMORPHIC : s <- s o exp(u)
     *   UPDATE_ADDITIVE      : s <- s + u        (ITK basic)
     *   UPDATE_COMPOSITIVE   : s <- s o (Id+u)   (Thirion's proposal?)
     * @param  value  update rule
     */
    void                        SetUpdateRule(UpdateRule value);


    /**
     * Gets the gradient type used for computing the demons force.
     * @return  gradient type
     */
    GradientType                GetGradientType(void) const;


    /**
     * Gets the gradient type used for computing the demons force as string.
     * @return  string describing gradient type
     */
    std::string                 GetGradientTypeAsString(void) const;


    /**
     * Sets the gradient type used for computing the demons force.
     * @param  value  gradient type
     */
    void                        SetGradientType(GradientType value);


    /**
     * Gets the maximum length of an update vector (voxel units).
     * @return  maximum length
     */
    float                       GetMaximumUpdateStepLength(void) const;


    /**
     * Sets the maximum length of an update vector (voxel units).
     * Setting it to 0 implies no restrictions will be made on the step length.
     * @param  value  maximum length
     */
    void                        SetMaximumUpdateStepLength(float value);


    /**
     * Gets the standard deviation (voxel unit) of the update field (Gaussian) smoothing.
     * Setting it below 0.1 means no smoothing will be performed.
     * @return  standard deviation
     */
    float                       GetUpdateFieldStandardDeviation(void) const;


    /**
     * Sets the standard deviation (voxel unit) of the update field (Gaussian) smoothing.
     * @param  value  standard deviation
     */
    void                        SetUpdateFieldStandardDeviation(float value);


    /**
     * Gets the standard deviation (voxel unit) of the displacement field (Gaussian) smoothing.
     * Setting it below 0.1 means no smoothing will be performed.
     * @return  standard deviation
     */
    float                       GetDisplacementFieldStandardDeviation(void) const;


    /**
     * Sets the standard deviation (voxel unit) of the displacement field (Gaussian) smoothing.
     * @param  value  standard deviation
     */
    void                        SetDisplacementFieldStandardDeviation(float value);


    /**
     * Does the algorithm uses histogram matching before processing?
     * @return  true if histogram matching is used, and false otherwise.
     */
    bool                        GetUseHistogramMatching(void) const;


    /**
     * Sets if the algorithm uses histogram matching before processing or not?
     * @param  value  true if histogram matching sould be used, and false otherwise.
     */
    void                        SetUseHistogramMatching(bool value);


    /**
     * Gets the initial transformation.
     * @return  initial transformation
     */
    TransformPointerType        GetInitialTransformation(void) const;


    /**
     * Sets the initial transformation.
     * @param  transform  initial transformation
     */
    void                        SetInitialTransformation(TransformType * transform);


    /**
     * Performs the image registration. Must be called before GetTransformation().
     */
    virtual void                StartRegistration(void);

};


} // End of namespace


/** Add the source code file (template) */
#include "rpiDiffeomorphicDemons.cxx"


#endif // _RPI_DIFFEOMORPHIC_DEMONS_HXX_
