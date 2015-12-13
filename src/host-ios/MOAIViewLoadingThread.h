//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "MOAICommandThread.h"

//================================================================//
// MOAIViewLoadingThread
//================================================================//
@interface MOAIViewLoadingThread : MOAICommandThread {
@private
}

    //----------------------------------------------------------------//
    -( void )           create                      :( EAGLContext* )eaglContext;
	-( void )           load;

@end
