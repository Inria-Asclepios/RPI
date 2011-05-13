#ifdef WIN32
    #ifdef RPI_EXPORTS
        #define RPI_EXPORT __declspec(dllexport) 
    #else
        #define RPI_EXPORT __declspec(dllimport) 
    #endif
#else
    #define RPI_EXPORT	
#endif
