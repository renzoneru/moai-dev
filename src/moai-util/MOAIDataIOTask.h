// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATAIOTASK_H
#define MOAIDATAIOTASK_H

#include <moai-util/MOAITask.h>

class MOAIDataBuffer;

//================================================================//
// MOAIDataIOTask
//================================================================//
class MOAIDataIOTask : 
	public MOAITask {
private:

	STLString						mFilename;
	MOAIDataBuffer*					mData;
	MOAILuaStrongRef				mOnFinish;
	u32								mAction;
	
	bool							mInflateOnLoad;
	bool							mInflateOnTaskThread;
	int								mWindowBits;

	//----------------------------------------------------------------//
	void		Execute				();
	void		Publish				();

public:

	enum {
		NONE,
		LOAD_ACTION,
		SAVE_ACTION,
	};

	//----------------------------------------------------------------//
	void		Init				( cc8* filename, MOAIDataBuffer& target, u32 action );
				MOAIDataIOTask		();
				~MOAIDataIOTask		();
	void		RegisterLuaClass	( MOAILuaState& state );
	void		RegisterLuaFuncs	( MOAILuaState& state );
	void		SaveData			( cc8* filename, MOAIDataBuffer& target );
	void		SetCallback			( lua_State* L, int idx );
	void		SetInflateOnLoad	( bool inflateOnLoad, bool inflateOnTaskThread, int windowBits );
};

#endif
