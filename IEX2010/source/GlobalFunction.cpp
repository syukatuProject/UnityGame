
#include	"iextreme.h"
//#include	"Camera.h"
//#include	"DrawShape.h"
#include	"GlobalFunction.h"

//*****************************************************************************
//
//	�O���[�o���֐�
//
//*****************************************************************************

//----------------------------------------------------------------------------
//	�V�X�e��
//----------------------------------------------------------------------------

	//	�ϐ��ϊ�
	POINT	GetPoint( int x, int y )
	{
		POINT	p;
		p.x = x;
		p.y = y;
		return p;
	}

	DWORD GetColor( float r, float g, float b, float a )
	{
		DWORD	ret;
		ret = ARGB( ( DWORD )( 0xFF * a ), ( DWORD )( 0xFF * r ), ( DWORD )( 0xFF * g ), ( DWORD )( 0xFF * b ) );
		return	ret;
	}

	DWORD GetColor( Vector3 color )
	{
		DWORD	ret;
		ret = ARGB( 0xFF, ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) );
		return ret;
	}

	DWORD GetColor( Vector3 color, float alpha )
	{
		DWORD	ret;
		ret = ARGB( ( DWORD )( 0xFF * alpha ), ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) );
		return ret;
	}

	//	���[���h���W����X�N���[�����W�ɕϊ�
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat )
	{
		//	���_�~�s��Ŏˉe����
		Vector3	temp;
		temp.x = pos.x *mat._11 + pos.y * mat._21 + pos.z * mat._31 + 1.0f * mat._41;
		temp.y = pos.x *mat._12 + pos.y * mat._22 + pos.z * mat._32 + 1.0f * mat._42;
		temp.z = pos.x *mat._13 + pos.y * mat._23 + pos.z * mat._33 + 1.0f * mat._43;
		float w = pos.x *mat._14 + pos.y * mat._24 + pos.z * mat._34 + 1.0f * mat._44;
		temp.x /= w;
		temp.y /= w;
		temp.z /= w;

		//	��ʍ��W�Ɋ��Z(�r���[�|�[�g�ϊ�)
		long WinWide, WinHeight;
		WinWide = iexSystem::ScreenWidth;
		WinWide /= 2;
		WinHeight = iexSystem::ScreenHeight;
		WinHeight /= 2;

		out.x = ( temp.x *WinWide + WinWide );
		out.y = ( -temp.y*WinHeight + WinHeight );
		out.z = temp.z;

		if ( temp.x >  1.0f ) return false;
		if ( temp.x < -1.0f ) return false;
		if ( temp.y >  1.0f ) return false;
		if ( temp.y < -1.0f ) return false;
		if ( temp.z <  0 )	return false;
		return true;
	}

	//	�QD���W����RD���W�ւ̕ϊ�
	void	TransCoordScreenToWorld( const Vector3& screenPos, Vector3& out )
	{
		D3DXVECTOR3	d3dxScreenCoord( screenPos.x, screenPos.y, screenPos.z );

		//	�r���[�|�[�g
		D3DVIEWPORT9	viewPort;
		iexSystem::Device->GetViewport( &viewPort );

		//	���[���h�ϊ�
		D3DXMATRIX	worldMatrix;
		D3DXMatrixIdentity( &worldMatrix );

		//	�ϊ�
		D3DXVECTOR3	result;
		D3DXVec3Unproject( &result, &d3dxScreenCoord, &viewPort, &matProjection, &matView, &worldMatrix );
		
		//	�o��
		out = Vector3( result.x, result.y, result.z );
	}

	//	�X�N���[�����W���烏�[���h���W�֕ϊ�
	void	ClientToWorld( Vector3& outPos, Vector3& outVec, POINT screenPos )
	{
		//	�X�N���[�����W���烏�[���h���W�����߂�
		Vector3	nearPoint;
		TransCoordScreenToWorld( Vector3( ( float )screenPos.x, ( float )screenPos.y, 0.1f ), nearPoint );
		Vector3	farPoint;
		TransCoordScreenToWorld( Vector3( ( float )screenPos.x, ( float )screenPos.y, 0.9f ), farPoint );
		outPos = nearPoint;
		Vector3	vEye = farPoint - nearPoint;
		vEye.Normalize();

		outVec = vEye;
	}

	//	�X�N���[�����W���烏�[���h���W�֕ϊ��i ���C�𓖂Ă郁�b�V���A�o�́A�X�N���[�����W �j
	void	ClientToWorld( iexMesh* targetMesh, Vector3& out, const POINT& screenPos )
	{
		//	�ϐ�����
		Vector3	pos, vec;
		float dist = FLT_MAX;

		//	���[���h���W�����߂�
		ClientToWorld( pos, vec, screenPos );

		//	���b�V���փ��C���΂��A�����������W��������
		targetMesh->RayPick( &out, &pos, &vec, &dist );
	}

	//	�O�t���[������̌o�ߎ��Ԏ擾
	float	GetElapseTime( void )
	{
		//	Windows�N������̌o�ߎ��Ԃ��擾
		static DWORD lastTime = timeGetTime();
		
		//	�O�t���[������̌o�ߎ��Ԃ��Z�o
		DWORD elapseTime = timeGetTime() - lastTime;

		//	���t���[���̌o�ߎ��Ԃ����Z
		lastTime += elapseTime;
		
		//	�o�ߎ��Ԃ�������
		return	( float )elapseTime / 1000.0f;
	}

//----------------------------------------------------------------------------
//	�������x�W�F�Ȑ��p�����[�^�擾( 0.0f ~ 1.0f )
//----------------------------------------------------------------------------

	namespace
	{
		const static int nPasTgl[12][12] = {
			{ 1 },
			{ 1, 1 },
			{ 1, 2, 1 },
			{ 1, 3, 3, 1 },
			{ 1, 4, 6, 4, 1 },
			{ 1, 5, 10, 10, 5, 1 },
			{ 1, 6, 15, 20, 15, 6, 1 },
			{ 1, 7, 21, 35, 35, 21, 7, 1 },
			{ 1, 8, 28, 56, 70, 56, 28, 8, 1 },
			{ 1, 9, 36, 84, 126, 126, 84, 36, 9, 1 },
			{ 1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1 },
			{ 1, 11, 55, 165, 330, 464, 464, 330, 165, 55, 11, 1 }
		};
	}

	inline float Get( const float &y1, const float &y2, const float &t, const int &n ){
		float b = t > 1 ? 1 : (t < 0 ? 0 : t);
		float a = 1 - b;
		float ay = 0;
		float y[4] = { 0, y1, y2, 1 };
		int m;
		for (int i = 0; i <= n; i++){
			m = i == 0 ? 0 : (i == n ? 3 : (i <= n / 2 ? 1 : 2));//y�̓Y��������
			ay += nPasTgl[n][i] * pow(a, n - i)*pow(b, i)*y[m];//��1
		}
		return ay;
	}

	//	�x�W�F�Ȑ��p�����[�^�擾�i 0.0f ~ 1.0f �ŕԂ� �j�@
	float GetBezier( int ePrm1, int ePrm2, float fRate ){
		int n = 3;                //n�����w��
		float y1, y2;
		switch (ePrm1){
		case ePrm_t::eSlow_Lv5: y1 = 0;						n = 11; break;//11����
		case ePrm_t::eSlow_Lv4: y1 = 0;                        n = 9; break;//9����
		case ePrm_t::eSlow_Lv3: y1 = 0;                        n = 7; break;//7����
		case ePrm_t::eSlow_Lv2: y1 = 0;                        n = 5; break;//5����
		case ePrm_t::eSlow_Lv1: y1 = 0;                        n = 3; break;//3����
		case ePrm_t::eNoAccel: y1 = 0.333333f;				n = 3; break;//�����̏ꍇ��3������1/3�̓_
		case ePrm_t::eRapid_Lv1: y1 = 1;						n = 3; break;//3����
		case ePrm_t::eRapid_Lv2: y1 = 1;						n = 5; break;//5����
		case ePrm_t::eRapid_Lv3: y1 = 1;						n = 7; break;//7����
		case ePrm_t::eRapid_Lv4: y1 = 1;						n = 9; break;//9����
		case ePrm_t::eRapid_Lv5: y1 = 1;						n = 11; break;//11����
		}
		switch (ePrm2){
		case ePrm_t::eSlow_Lv5: y2 = 1;                       n = 11; break;//11����
		case ePrm_t::eSlow_Lv4: y2 = 1;                       n = 9; break;//9����
		case ePrm_t::eSlow_Lv3: y2 = 1;                       n = 7; break;//7����
		case ePrm_t::eSlow_Lv2: y2 = 1;                       n = 5; break;//5����
		case ePrm_t::eSlow_Lv1: y2 = 1;                       n = 3; break;//3����
		case ePrm_t::eNoAccel: y2 = 0.6666667f;              n = 3; break;//�����̏ꍇ��3������2/3�̓_
		case ePrm_t::eRapid_Lv1: y2 = 0;                       n = 3; break;//3����
		case ePrm_t::eRapid_Lv2: y2 = 0;                       n = 5; break;//5����
		case ePrm_t::eRapid_Lv3: y2 = 0;                       n = 7; break;//7����
		case ePrm_t::eRapid_Lv4: y2 = 0;                       n = 9; break;//9����
		case ePrm_t::eRapid_Lv5: y2 = 0;                       n = 11; break;//11����
		}

		float	out = Get( y1, y2, fRate, n );
		return out;
	}

//----------------------------------------------------------------------------
//	�}�`�ݒ�
//----------------------------------------------------------------------------

	//	���_�ݒ�
	void	SetVertex( LVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color )
	{
		v.x = x;
		v.y = y;
		v.z = z;
		v.tu = tu;
		v.tv = tv;
		v.color = color;
	}

	//	���_������
	void	SetVertex( TLVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color )
	{
		v.sx = x;
		v.sy = y;
		v.sz = z;
		v.tu = tu;
		v.tv = tv;
		v.color = color;
		v.rhw = 1.0f;
	}

//----------------------------------------------------------------------
//	������(�o�́A�J�n�_�A�ڕW�n�_�A�����A�����镉��(�d��))
//----------------------------------------------------------------------
	bool Parabola(Vector3& out, Vector3 start, Vector3 end, float speed, float mass)
	{
		float angleTan;
		float elevation;	//�p
		float height = start.y - end.y;	//���፷
		float tx = end.x - start.x;	//�@X���W��
		float tz = end.z - start.z;	//�@Z���W��

		float txz = sqrtf( tx * tx + tz * tz );

		// y = v0 * sin��*t + 0.5f*G*t*t
		// x = v0 * cos��*t
		// 2�������� a*T*T + b*T + c = 0
		float A = (mass*txz*txz) / (2.0f*speed*speed);

		// T�̌W��(aT*T + b*T + c = 0)	
		float a, b, c;
		a = 1.0f;
		if (A != 0){
			b = txz / A;
			c = 1.0f + height / A;
		}
		// ���̌���(������)
		float D = b * b - 4 * a * c;
		if (D < 0)
		{
			out = Vector3(0, 0, 0);
			angleTan = 0;
			return false;
		}

		// ���̌���
		elevation = (-b + sqrtf(D)) / (2.0f*a);

		// �p
		angleTan = atanf(elevation);

		// �ړ�����
		D3DXVECTOR3 tt;
		tt.x = end.x - start.x;
		tt.y = 0;
		tt.z = end.z - start.z;
		D3DXVec3Normalize(&tt, &tt);

		out.x = speed * cosf(angleTan)*tt.x;
		out.y = speed * sinf(angleTan);
		out.z = speed * cosf(angleTan)*tt.z;

		return true;
	}

//-------------------------------------------------------------------------
//	���ݕϊ�DirectX<->IEX
//-------------------------------------------------------------------------

	void	QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ )
	{
		pOut->x = pQ->x;
		pOut->y = pQ->y;
		pOut->z = pQ->z;
		pOut->w = pQ->w;
	}

	void	QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ )
	{
		pOut->x = pQ->x;
		pOut->y = pQ->y;
		pOut->z = pQ->z;
		pOut->w = pQ->w;
	}

	void	Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV )
	{
		pOut->x = pV->x;
		pOut->y = pV->y;
		pOut->z = pV->z;
	}

	void	Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV )
	{
		pOut->x = pV->x;
		pOut->y = pV->y;
		pOut->z = pV->z;
	}

//-------------------------------------------------------------------------
//	���Z�n
//-------------------------------------------------------------------------
	
	//�@( min <= value && value < max )
	bool isMiddle(const int& value, const int& min, const int& max)
	{
		return	(min <= value && value < max);
	}
	bool isMiddle(const float& value, const float& min, const float& max)
	{
		return	(min <= value && value < max);
	}