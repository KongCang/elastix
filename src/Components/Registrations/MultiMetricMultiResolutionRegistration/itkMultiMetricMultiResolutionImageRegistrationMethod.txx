#ifndef _itkMultiMetricMultiResolutionImageRegistrationMethod_txx
#define _itkMultiMetricMultiResolutionImageRegistrationMethod_txx

#include "itkMultiMetricMultiResolutionImageRegistrationMethod.h"

/** macro that implements the set methods */
#define itkImplementationSetMacro(_name,_type) \
  template < typename TFixedImage, typename TMovingImage > \
  void \
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage> \
  ::Set##_name ( _type _arg, unsigned int pos ) \
  { \
    if (pos == 0) \
    { \
      this->Superclass::Set##_name ( _arg ); \
    } \
    if ( pos >= this->GetNumberOf##_name##s() ) \
    { \
      this->SetNumberOf##_name##s( pos+1 ); \
    } \
    if ( this->m_##_name##s [pos] != _arg ) \
    { \
      this->m_##_name##s [pos] = _arg; \
      this->Modified(); \
    } \
  } // comment to allow ; after calling macro



namespace itk
{
  itkImplementationSetMacro(FixedImage, const FixedImageType *);
  itkImplementationSetMacro(MovingImage, const MovingImageType *);
  itkImplementationSetMacro(FixedImageRegion, const FixedImageRegionType);
  itkImplementationSetMacro(Interpolator, InterpolatorType *);
  itkImplementationSetMacro(FixedImagePyramid, FixedImagePyramidType *);
  itkImplementationSetMacro(MovingImagePyramid, MovingImagePyramidType *);

  /**
   * ****************** Constructor ******************
   */
  template < typename TFixedImage, typename TMovingImage >
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::MultiMetricMultiResolutionImageRegistrationMethod()
  {
    this->SetMetric( CombinationMetricType::New() );
    this->m_Stop = false;
    this->m_LastTransformParameters = ParametersType(1);
    this->m_LastTransformParameters.Fill( 0.0f );
  }

  /** 
   * **************** GetFixedImage **********************************
   */

  template < typename TFixedImage, typename TMovingImage >
  const typename 
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::FixedImageType *
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GetFixedImage( unsigned int pos ) const
  {
    if ( pos >= this->GetNumberOfFixedImages() )
    {
      return 0;
    }
    else
    {
      return this->m_FixedImages[ pos ].GetPointer();
    }
  } // end GetFixedImage


  /** 
   * **************** GetMovingImage **********************************
   */

  template < typename TFixedImage, typename TMovingImage >
  const typename 
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::MovingImageType *
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GetMovingImage( unsigned int pos ) const
  {
    if ( pos >= this->GetNumberOfMovingImages() )
    {
      return 0;
    }
    else
    {
      return this->m_MovingImages[ pos ].GetPointer();
    }
  } // end GetMovingImage


  /** 
   * **************** GetInterpolator **********************************
   */

  template < typename TFixedImage, typename TMovingImage >
  typename 
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::InterpolatorType *
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GetInterpolator( unsigned int pos ) const
  {
    if ( pos >= this->GetNumberOfInterpolators() )
    {
      return 0;
    }
    else
    {
      return this->m_Interpolators[ pos ].GetPointer();
    }
  } // end GetInterpolator


  /** 
   * **************** GetFixedImagePyramid **********************************
   */

  template < typename TFixedImage, typename TMovingImage >
  typename 
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::FixedImagePyramidType *
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GetFixedImagePyramid( unsigned int pos ) const
  {
    if ( pos >= this->GetNumberOfFixedImagePyramids() )
    {
      return 0;
    }
    else
    {
      return this->m_FixedImagePyramids[ pos ].GetPointer();
    }
  } // end GetFixedImagePyramid


  /** 
   * **************** GetMovingImagePyramid **********************************
   */

  template < typename TFixedImage, typename TMovingImage >
  typename 
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::MovingImagePyramidType *
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GetMovingImagePyramid( unsigned int pos ) const
  {
    if ( pos >= this->GetNumberOfMovingImagePyramids() )
    {
      return 0;
    }
    else
    {
      return this->m_MovingImagePyramids[ pos ].GetPointer();
    }
  } // end GetMovingImagePyramid


  /** 
   * **************** GetFixedImageRegion **********************************
   */

  template < typename TFixedImage, typename TMovingImage >
  const typename 
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::FixedImageRegionType &
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GetFixedImageRegion( unsigned int pos ) const
  {
    if ( pos >= this->GetNumberOfFixedImageRegions() )
    {
      /** Return a dummy fixed image region */
      return this->m_NullFixedImageRegion;
    }
    else
    {
      return this->m_FixedImageRegions[ pos ];
    }
  } // end GetFixedImageRegion


  /**
   * ********************** SetMetric *******************************
   * Reimplement this method to check if
   * the metric is a combination metric.
   * GetMetric returns the combination metric.
   */
  
  template < typename TFixedImage, typename TMovingImage >
  void 
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::SetMetric( MetricType * _arg )
  {
    CombinationMetricType * testPtr = dynamic_cast<CombinationMetricType *>( _arg );
    if ( testPtr )
    {
      if ( this->m_CombinationMetric != testPtr )
      {
        this->m_CombinationMetric = testPtr;
        this->Superclass::SetMetric( this->m_CombinationMetric );
        this->Modified();
      }
    }
    else
    {
      itkExceptionMacro( << "The metric must of type CombinationImageToImageMetric!" );
    }
  } // end SetMetric


  /*
  * ****************** Initialize *******************************
  */

  template < typename TFixedImage, typename TMovingImage >
  void
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::Initialize() throw (ExceptionObject)
  {
    this->CheckOnInitialize();

    // Setup the metric
    this->GetCombinationMetric()->SetTransform( this->GetTransform() );

    this->GetCombinationMetric()->SetMovingImage(
      this->GetMovingImagePyramid()->GetOutput( this->GetCurrentLevel() ) );
    for ( unsigned int i = 0; i < this->GetNumberOfMovingImagePyramids(); ++i)
    {
      this->GetCombinationMetric()->SetMovingImage(
        this->GetMovingImagePyramid(i)->GetOutput( this->GetCurrentLevel() ), i );
    }

    this->GetCombinationMetric()->SetFixedImage(
      this->GetFixedImagePyramid()->GetOutput( this->GetCurrentLevel() ) );
    for ( unsigned int i = 0; i < this->GetNumberOfFixedImagePyramids(); ++i)
    {
      this->GetCombinationMetric()->SetFixedImage(
        this->GetFixedImagePyramid(i)->GetOutput( this->GetCurrentLevel() ), i );
    }
    
    this->GetCombinationMetric()->SetInterpolator( this->GetInterpolator() );
    for ( unsigned int i = 0; i < this->GetNumberOfInterpolators(); ++i)
    {
      this->GetCombinationMetric()->SetInterpolator( this->GetInterpolator(i), i );
    }
    
    this->GetCombinationMetric()->SetFixedImageRegion(
      this->m_FixedImageRegionPyramids[0][ this->GetCurrentLevel() ] );
    for ( unsigned int i = 0; i < this->m_FixedImageRegionPyramids.size(); ++i)
    {
      this->GetCombinationMetric()->SetFixedImageRegion(
        this->m_FixedImageRegionPyramids[i][ this->GetCurrentLevel() ], i );
    }

    this->GetMetric()->Initialize();

    // Setup the optimizer
    this->GetOptimizer()->SetCostFunction( this->GetMetric() );
    this->GetOptimizer()->SetInitialPosition(
      this->GetInitialTransformParametersOfNextLevel() );

    /** Connect the transform to the Decorator. */
    TransformOutputType * transformOutput =  
      static_cast< TransformOutputType * >( this->ProcessObject::GetOutput(0) );

    transformOutput->Set( this->GetTransform() );
   
  } // end Initialize


  /*
   * ****************** Prepare all pyramids ******************
   */

  template < typename TFixedImage, typename TMovingImage >
  void
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::PrepareAllPyramids( void )
  {
    this->CheckPyramids();

    /** Setup the moving image pyramids */
    for (unsigned int i = 0; i < this->GetNumberOfMovingImagePyramids(); ++i )
    {
      MovingImagePyramidPointer movpyr = this->GetMovingImagePyramid(i);
      if ( movpyr.IsNotNull() )
      {
        movpyr->SetNumberOfLevels( this->GetNumberOfLevels() );
        if ( this->GetNumberOfMovingImages() > 1 )
        {
          movpyr->SetInput( this->GetMovingImage(i) );
        }
        else
        {
          movpyr->SetInput( this->GetMovingImage() );
        }
        movpyr->UpdateLargestPossibleRegion();
      }
    }

    /** Set up the fixed image pyramids and the fixed image region pyramids */
    typedef typename FixedImageRegionType::SizeType         SizeType;
    typedef typename FixedImageRegionType::IndexType        IndexType;
    typedef typename FixedImagePyramidType::ScheduleType    ScheduleType;

    this->m_FixedImageRegionPyramids.resize( this->GetNumberOfFixedImagePyramids() );

    for (unsigned int i = 0; i < this->GetNumberOfFixedImagePyramids(); ++i )
    {
      // Setup the fixed image pyramid
      FixedImagePyramidPointer fixpyr = this->GetFixedImagePyramid(i);
      if ( fixpyr.IsNotNull() ) 
      {
        fixpyr->SetNumberOfLevels( this->GetNumberOfLevels() );
        if ( this->GetNumberOfFixedImages() > 1 )
        {
          fixpyr->SetInput( this->GetFixedImage(i) );          
        }
        else
        {
          fixpyr->SetInput( this->GetFixedImage() );          
        }
        fixpyr->UpdateLargestPossibleRegion();

        ScheduleType schedule = fixpyr->GetSchedule();

        FixedImageRegionType fixedImageRegion;
        if (this->GetNumberOfFixedImageRegions() > 1)
        {
          fixedImageRegion = this->GetFixedImageRegion(i);
        }
        else
        {
          fixedImageRegion = this->GetFixedImageRegion();
        }
        SizeType  inputSize  = fixedImageRegion.GetSize();
        IndexType inputStart = fixedImageRegion.GetIndex();

        this->m_FixedImageRegionPyramids[i].reserve( this->GetNumberOfLevels() );
        this->m_FixedImageRegionPyramids[i].resize( this->GetNumberOfLevels() );
        
        // Compute the FixedImageRegion corresponding to each level of the 
        // pyramid. This uses the same algorithm of the ShrinkImageFilter 
        // since the regions should be compatible. 
        // \todo: better would be to set the region corners to the same 
        // physical coordinates as the fixed image region.
        for ( unsigned int level=0; level < this->GetNumberOfLevels(); level++ )
        {
          SizeType  size;
          IndexType start;
          for ( unsigned int dim = 0; dim < FixedImageType::ImageDimension; dim++)
          {
            const float scaleFactor = static_cast<float>( schedule[ level ][ dim ] );

            size[ dim ] = static_cast<typename SizeType::SizeValueType>(
              vcl_floor(static_cast<float>( inputSize[ dim ] ) / scaleFactor ) );
            if( size[ dim ] < 1 )
            {
              size[ dim ] = 1;
            }
            
            start[ dim ] = static_cast<typename IndexType::IndexValueType>(
              vcl_ceil(static_cast<float>( inputStart[ dim ] ) / scaleFactor ) ); 
          }
          this->m_FixedImageRegionPyramids[ i ][ level ].SetSize( size );
          this->m_FixedImageRegionPyramids[ i ][ level ].SetIndex( start );
        } // end for loop over res levels

      } // end if fixpyr!=0

    } // end for loop over fixed pyramids
 
  } // end PrepareAllPyramids


  /*
   * ****************** PrintSelf ******************
   */

  template < typename TFixedImage, typename TMovingImage >
  void
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf( os, indent );
  } // end PrintSelf


  /*
   * ********************* Generate Data ***********************
   */

  template < typename TFixedImage, typename TMovingImage >
  void
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GenerateData()
  {
    this->m_Stop = false;

    /** Check the transform and set the initial parameters */
    if( this->GetTransform() == 0 )
    {
      itkExceptionMacro(<<"Transform is not present");
    }

    this->SetInitialTransformParametersOfNextLevel( this->GetInitialTransformParameters() );

    if ( this->GetInitialTransformParametersOfNextLevel().Size() != 
        this->GetTransform()->GetNumberOfParameters() )
    {
      itkExceptionMacro(<<"Size mismatch between initial parameter and transform"); 
    }
    
    /** Prepare the fixed and moving pyramids */
    this->PrepareAllPyramids();
    
    /** Loop over the resolution levels */
    for ( unsigned int currentLevel = 0; currentLevel < this->GetNumberOfLevels();
          currentLevel++ )
    {  
      this->SetCurrentLevel( currentLevel );
      
      // Invoke an iteration event.
      // This allows a UI to reset any of the components between
      // resolution level.
      this->InvokeEvent( IterationEvent() );
      
      // Check if there has been a stop request
      if ( this->m_Stop ) 
      {
        break;
      }
      
      try
      {
        // initialize the interconnects between components
        this->Initialize();
      }
      catch( ExceptionObject& err )
      {
        this->m_LastTransformParameters = ParametersType(1);
        this->m_LastTransformParameters.Fill( 0.0f );
        
        // pass exception to caller
        throw err;
      }
      
      try
      {
        // do the optimization
        this->GetOptimizer()->StartOptimization();
      }
      catch( ExceptionObject& err )
      {
        // An error has occurred in the optimization.
        // Update the parameters
        this->m_LastTransformParameters = this->GetOptimizer()->GetCurrentPosition();
        
        // Pass exception to caller
        throw err;
      }
      
      // get the results
      this->m_LastTransformParameters = this->GetOptimizer()->GetCurrentPosition();
      this->GetTransform()->SetParameters( this->m_LastTransformParameters );
      
      // setup the initial parameters for next level
      if ( this->GetCurrentLevel() < this->GetNumberOfLevels() - 1 )
      {
        this->SetInitialTransformParametersOfNextLevel(
          this->m_LastTransformParameters );
      }

    } // end for loop over res levels

  } // end GenerateData


  /** 
   * ***************** GetMTime ******************
   */

  template < typename TFixedImage, typename TMovingImage >
  unsigned long
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::GetMTime() const
  {
    unsigned long mtime = Superclass::GetMTime();
    unsigned long m;

    // Some of the following should be removed once ivars are put in the
    // input and output lists

    for ( unsigned int i = 0; i < this->GetNumberOfInterpolators(); ++i )
    {
      InterpolatorPointer interpolator = this->GetInterpolator(i);
      if ( interpolator )
      {
        m = interpolator->GetMTime();
        mtime = (m > mtime ? m : mtime);
      }
    }

    for ( unsigned int i = 0; i < this->GetNumberOfFixedImages(); ++i )
    {
      FixedImageConstPointer fixedImage = this->GetFixedImage(i);
      if ( fixedImage )
      {
        m = fixedImage->GetMTime();
        mtime = (m > mtime ? m : mtime);
      }
    }
    
    for ( unsigned int i = 0; i < this->GetNumberOfMovingImages(); ++i )
    {
      MovingImageConstPointer movingImage = this->GetMovingImage(i);
      if ( movingImage )
      {
        m = movingImage->GetMTime();
        mtime = (m > mtime ? m : mtime);
      }
    }

    for ( unsigned int i = 0; i < this->GetNumberOfFixedImagePyramids(); ++i )
    {
      FixedImagePyramidPointer fixedImagePyramid = this->GetFixedImagePyramid(i);
      if ( fixedImagePyramid )
      {
        m = fixedImagePyramid->GetMTime();
        mtime = (m > mtime ? m : mtime);
      }
    }
   
    for ( unsigned int i = 0; i < this->GetNumberOfMovingImagePyramids(); ++i )
    {
      MovingImagePyramidPointer movingImagePyramid = this->GetMovingImagePyramid(i);
      if ( movingImagePyramid )
      {
        m = movingImagePyramid->GetMTime();
        mtime = (m > mtime ? m : mtime);
      }
    }

    return mtime;
    
  } // end GetMTime


  /*
   * ****************** CheckPyramids ******************
   */

  template < typename TFixedImage, typename TMovingImage >
  void
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::CheckPyramids( void ) throw (ExceptionObject)
  {
    /** Check if at least one of the following are provided */
    if( this->GetFixedImage() ==0 )
    {
      itkExceptionMacro(<<"FixedImage is not present");
    }
    if( this->GetMovingImage() ==0 )
    {
      itkExceptionMacro(<<"MovingImage is not present");
    }
    if( this->GetFixedImagePyramid() ==0 )
    {
      itkExceptionMacro(<<"Fixed image pyramid is not present");
    }
    if( this->GetMovingImagePyramid() ==0 )
    {
      itkExceptionMacro(<<"Moving image pyramid is not present");
    }
    
    /** Check if the number if fixed/moving pyramids >= nr of fixed/moving images,
     * and whether the number of fixed image regions == the number of fixed images */
    if ( this->GetNumberOfFixedImagePyramids() < this->GetNumberOfFixedImages() )
    {
      itkExceptionMacro( << "The number of fixed image pyramids should be >= the number of fixed images" );
    }
    if ( this->GetNumberOfMovingImagePyramids() < this->GetNumberOfMovingImages() )
    {
      itkExceptionMacro( << "The number of moving image pyramids should be >= the number of moving images" );
    }
    if ( this->GetNumberOfFixedImageRegions() != this->GetNumberOfFixedImages() )
    {
      itkExceptionMacro( << "The number of fixed image regions should equal the number of fixed image" );
    }

  } // end CheckPyramids

  
  /*
   * ****************** CheckOnInitialize ******************
   */

  template < typename TFixedImage, typename TMovingImage >
  void
  MultiMetricMultiResolutionImageRegistrationMethod<TFixedImage,TMovingImage>
  ::CheckOnInitialize( void ) throw (ExceptionObject)
  {
    /** check if at least one of the following is present */
    if ( this->GetMetric() == 0 )
    {
      itkExceptionMacro(<<"Metric is not present" );
    }
    if ( this->GetOptimizer() == 0 )
    {
      itkExceptionMacro(<<"Optimizer is not present" );
    }
    if( this->GetTransform() == 0 )
    {
      itkExceptionMacro(<<"Transform is not present");
    }
    if( this->GetInterpolator() == 0 )
    {
      itkExceptionMacro(<<"Interpolator is not present");
    }

    /** nrofmetrics >= nrofinterpolators >= nrofpyramids >= nofimages */
    unsigned int nrOfMetrics = this->GetCombinationMetric()->GetNumberOfMetrics();
    if ( this->GetNumberOfInterpolators() > nrOfMetrics )
    {
      itkExceptionMacro( << "NumberOfInterpolators can not exceed the NumberOfMetrics in the CombinationMetric!" );
    }
    if ( this->GetNumberOfFixedImagePyramids() > nrOfMetrics )
    {
      itkExceptionMacro( << "NumberOfFixedImagePyramids can not exceed the NumberOfMetrics in the CombinationMetric!" );
    }
    if ( this->GetNumberOfMovingImagePyramids() > nrOfMetrics )
    {
      itkExceptionMacro( << "NumberOfMovingImagePyramids can not exceed the NumberOfMetrics in the CombinationMetric!" );
    }
    if ( this->GetNumberOfMovingImagePyramids() >
      this->GetNumberOfInterpolators() )
    {
      itkExceptionMacro( << "NumberOfMovingImagePyramids can not exceed the NumberOfInterpolators!" );
    }

    /** for all components: ==nrofmetrics of ==1. */
    if (  ( this->GetNumberOfInterpolators() != 1 ) &&
          ( this->GetNumberOfInterpolators() != nrOfMetrics )  )
    {
      itkExceptionMacro( << "The NumberOfInterpolators should equal 1 or equal the NumberOfMetrics" );
    }
    if (  ( this->GetNumberOfFixedImagePyramids() != 1 ) &&
          ( this->GetNumberOfFixedImagePyramids() != nrOfMetrics )  )
    {
      itkExceptionMacro( << "The NumberOfFixedImagePyramids should equal 1 or equal the NumberOfMetrics" );
    }
    if (  ( this->GetNumberOfMovingImagePyramids() != 1 ) &&
          ( this->GetNumberOfMovingImagePyramids() != nrOfMetrics )  )
    {
      itkExceptionMacro( << "The NumberOfMovingImagePyramids should equal 1 or equal the NumberOfMetrics" );
    }
    if (  ( this->GetNumberOfFixedImages() != 1 ) &&
          ( this->GetNumberOfFixedImages() != nrOfMetrics )  )
    {
      itkExceptionMacro( << "The NumberOfFixedImages should equal 1 or equal the NumberOfMetrics" );
    }
    if (  ( this->GetNumberOfMovingImages() != 1 ) &&
          ( this->GetNumberOfMovingImages() != nrOfMetrics )  )
    {
      itkExceptionMacro( << "The NumberOfMovingImages should equal 1 or equal the NumberOfMetrics" );
    }
    
  } // end CheckOnInitialize


} // end namespace itk

#undef itkImplementationSetMacro

#endif
