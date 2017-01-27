// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAICollisionShape
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionShape::Bless () {

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {

		this->mBounds.Grow ( this->mShapes [ i ]->mBounds, i == 0 );
	}
}

//----------------------------------------------------------------//
void MOAICollisionShape::Clear () {

	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
		if ( this->mShapes [ i ]) {
			delete ( this->mShapes [ i ]);
		}
	}
	this->mShapes.Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Draw ( const ZLAffine3D& localToWorldMtx ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	gfxMgr.mVertexCache.SetUVTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::UV_TO_MODEL_MTX ));

	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	for ( u32 i = 0; i < this->mShapes.Size (); ++i ) {
	
		MOAIOverlapShape* shape = this->mShapes [ i ];
	
		switch ( shape->mType ) {
			
			case MOAIOverlap::BOX: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));
			
				ZLBox box = *( ZLBox* )shape->mPtr;
				box.Transform ( localToWorldMtx );
				draw.DrawBoxOutline ( box );
				break;
			}
			case MOAIOverlap::CIRCLE: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_CLIP_MTX ));
			
				ZLCircle circle = *( ZLCircle* )shape->mPtr;
				draw.DrawEllipseOutline ( circle.mCenter.mX, circle.mCenter.mY, circle.mRadius, circle.mRadius, 32 );
				break;
			}
			case MOAIOverlap::FRUSTUM:
				break;
			
			case MOAIOverlap::POLYGON:
				break;
			
			case MOAIOverlap::PRISM:
				break;
			
			case MOAIOverlap::QUAD: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::MODEL_TO_CLIP_MTX ));
				draw.DrawQuad ( *( ZLQuad* )shape->mPtr );
				break;
			}
			case MOAIOverlap::RECT: {
			
				gfxMgr.mVertexCache.SetVertexTransform ( gfxMgr.mGfxState.GetMtx ( MOAIGfxGlobalsCache::WORLD_TO_CLIP_MTX ));
			
				ZLRect rect = *( ZLRect* )shape->mPtr;
				localToWorldMtx.Transform ( rect );
				draw.DrawRectOutline ( rect );
				break;
			}
			case MOAIOverlap::SPHERE:
				break;
			
			default:
				break;
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionShape::FindOverlaps ( const ZLBox& otherBounds, MOAIOverlapHandler& handler  ) const {

	MOAIOverlapBox otherShape;
	otherShape.mShape = otherBounds;
	otherShape.mBounds = otherBounds;

	size_t selfShapeCount = this->mShapes.Size ();

	if ( selfShapeCount == 1 ) {
	
		MOAIOverlap::Overlap ( *this->mShapes [ 0 ], otherShape, handler );
	}
	else {
	
		for ( size_t i = 0; i < selfShapeCount; ++i ) {
			MOAIOverlap::Overlap ( *this->mShapes [ i ], otherShape, handler );
		}
	}
}

//----------------------------------------------------------------//
void MOAICollisionShape::FindOverlaps ( const MOAICollisionShape& otherShape, MOAIOverlapHandler& handler  ) const {

	size_t selfShapeCount = this->mShapes.Size ();
	size_t otherShapeCount = otherShape.mShapes.Size ();
	
	if (( selfShapeCount == 1 ) || ( otherShapeCount == 1 )) {
	
		if (( selfShapeCount == 1 ) && ( otherShapeCount == 1 )) {
		
			MOAIOverlap::Overlap ( *this->mShapes [ 0 ], *otherShape.mShapes [ 0 ], handler );
		}
		else if ( otherShapeCount == 1 ) {
		
			for ( size_t i = 0; i < selfShapeCount; ++i ) {
				MOAIOverlap::Overlap ( *this->mShapes [ i ], *otherShape.mShapes [ 0 ], handler );
			}
		}
		else {
		
			for ( size_t i = 0; i < otherShapeCount; ++i ) {
				MOAIOverlap::Overlap ( *this->mShapes [ 0 ], *otherShape.mShapes [ i ], handler );
			}
		}
	}
	else {
	
		for ( size_t i = 0; i < selfShapeCount; ++i ) {
			for ( size_t j = 0; j < otherShapeCount; ++j ) {
				MOAIOverlap::Overlap ( *this->mShapes [ i ], *otherShape.mShapes [ j ], handler );
			}
		}
	}
}

//----------------------------------------------------------------//
MOAICollisionShape::MOAICollisionShape () {

	this->mBounds = ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAICollisionShape::~MOAICollisionShape () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( u32 idx, const ZLBox& box ) {

	this->mShapes.Resize ( idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapBox* overlap = new MOAIOverlapBox ();
	overlap->mShape = box;
	overlap->mBounds.Init ( box );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( u32 idx, const ZLQuad& quad ) {

	this->mShapes.Resize ( idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapQuad* overlap = new MOAIOverlapQuad ();
	overlap->mShape = quad;
	overlap->mBounds.Init ( quad.GetBounds ());
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}

//----------------------------------------------------------------//
void MOAICollisionShape::Set ( u32 idx, const ZLRect& rect ) {

	this->mShapes.Resize ( idx + 1, 0 );
	if ( this->mShapes [ idx ]) {
		delete this->mShapes [ idx ];
	}
	MOAIOverlapRect* overlap = new MOAIOverlapRect ();
	overlap->mShape = rect;
	overlap->mBounds.Init ( rect );
	this->mShapes [ idx ] = overlap;
	
	this->Bless ();
}
