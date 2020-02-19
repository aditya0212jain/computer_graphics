#pragma once

#define OPENGLREVMAJOR	3
#define OPENGLREVMINOR	3

#ifdef __APPLE__

#if (OPENGLREVMAJOR > 2)
#define COREPROFILE	true
#else
#define COREPROFILE	false
#endif

#else // Not __APPLE__

#define COREPROFILE	false

#endif // if __APPLE__


#define FOV_DEFAULT     60.0
#define NEAR_DEFAULT	0.5
#define FAR_DEFAULT	1024.0
