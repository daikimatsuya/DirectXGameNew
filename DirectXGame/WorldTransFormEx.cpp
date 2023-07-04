#include "AffineMatrixFunctions.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include <d3d12.h>
#include <wrl.h>

AffineMatrixFunctions afm;
void WorldTransform::UpdateMatrix() {
	matWorld_ = afm.MakeAffinMatrix(
	    afm.MakeScaleMatrix(scale_),
	    afm.MakeRotateMatrix(
	        afm.MakeRotateXMatrix(rotation_.x), afm.MakeRotateYMatrix(rotation_.y),
	        afm.MakeRotateZMatrix(rotation_.z)),
	    afm.MakeTranslateMatrix(translation_));
	if (parent_) {
		matWorld_ = afm.Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}