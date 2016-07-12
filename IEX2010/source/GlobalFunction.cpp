
#include	"iextreme.h"
//#include	"Camera.h"
//#include	"DrawShape.h"
#include	"GlobalFunction.h"

//*****************************************************************************
//
//	グローバル関数
//
//*****************************************************************************

//----------------------------------------------------------------------------
//	システム
//----------------------------------------------------------------------------

	//	変数変換
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

	//	ワールド座標からスクリーン座標に変換
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat )
	{
		//	頂点×行列で射影する
		Vector3	temp;
		temp.x = pos.x *mat._11 + pos.y * mat._21 + pos.z * mat._31 + 1.0f * mat._41;
		temp.y = pos.x *mat._12 + pos.y * mat._22 + pos.z * mat._32 + 1.0f * mat._42;
		temp.z = pos.x *mat._13 + pos.y * mat._23 + pos.z * mat._33 + 1.0f * mat._43;
		float w = pos.x *mat._14 + pos.y * mat._24 + pos.z * mat._34 + 1.0f * mat._44;
		temp.x /= w;
		temp.y /= w;
		temp.z /= w;

		//	画面座標に換算(ビューポート変換)
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

	//	２D座標から３D座標への変換
	void	TransCoordScreenToWorld( const Vector3& screenPos, Vector3& out )
	{
		D3DXVECTOR3	d3dxScreenCoord( screenPos.x, screenPos.y, screenPos.z );

		//	ビューポート
		D3DVIEWPORT9	viewPort;
		iexSystem::Device->GetViewport( &viewPort );

		//	ワールド変換
		D3DXMATRIX	worldMatrix;
		D3DXMatrixIdentity( &worldMatrix );

		//	変換
		D3DXVECTOR3	result;
		D3DXVec3Unproject( &result, &d3dxScreenCoord, &viewPort, &matProjection, &matView, &worldMatrix );
		
		//	出力
		out = Vector3( result.x, result.y, result.z );
	}

	//	スクリーン座標からワールド座標へ変換
	void	ClientToWorld( Vector3& outPos, Vector3& outVec, POINT screenPos )
	{
		//	スクリーン座標からワールド座標を求める
		Vector3	nearPoint;
		TransCoordScreenToWorld( Vector3( ( float )screenPos.x, ( float )screenPos.y, 0.1f ), nearPoint );
		Vector3	farPoint;
		TransCoordScreenToWorld( Vector3( ( float )screenPos.x, ( float )screenPos.y, 0.9f ), farPoint );
		outPos = nearPoint;
		Vector3	vEye = farPoint - nearPoint;
		vEye.Normalize();

		outVec = vEye;
	}

	//	スクリーン座標からワールド座標へ変換（ レイを当てるメッシュ、出力、スクリーン座標 ）
	void	ClientToWorld( iexMesh* targetMesh, Vector3& out, const POINT& screenPos )
	{
		//	変数準備
		Vector3	pos, vec;
		float dist = FLT_MAX;

		//	ワールド座標を求める
		ClientToWorld( pos, vec, screenPos );

		//	メッシュへレイを飛ばし、当たった座標をかえす
		targetMesh->RayPick( &out, &pos, &vec, &dist );
	}

	//	前フレームからの経過時間取得
	float	GetElapseTime( void )
	{
		//	Windows起動からの経過時間を取得
		static DWORD lastTime = timeGetTime();
		
		//	前フレームからの経過時間を算出
		DWORD elapseTime = timeGetTime() - lastTime;

		//	現フレームの経過時間を加算
		lastTime += elapseTime;
		
		//	経過時間をかえす
		return	( float )elapseTime / 1000.0f;
	}

//----------------------------------------------------------------------------
//	高次元ベジェ曲線パラメータ取得( 0.0f ~ 1.0f )
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
			m = i == 0 ? 0 : (i == n ? 3 : (i <= n / 2 ? 1 : 2));//yの添え字決定
			ay += nPasTgl[n][i] * pow(a, n - i)*pow(b, i)*y[m];//※1
		}
		return ay;
	}

	//	ベジェ曲線パラメータ取得（ 0.0f ~ 1.0f で返す ）　
	float GetBezier( int ePrm1, int ePrm2, float fRate ){
		int n = 3;                //n次元指定
		float y1, y2;
		switch (ePrm1){
		case ePrm_t::eSlow_Lv5: y1 = 0;						n = 11; break;//11次元
		case ePrm_t::eSlow_Lv4: y1 = 0;                        n = 9; break;//9次元
		case ePrm_t::eSlow_Lv3: y1 = 0;                        n = 7; break;//7次元
		case ePrm_t::eSlow_Lv2: y1 = 0;                        n = 5; break;//5次元
		case ePrm_t::eSlow_Lv1: y1 = 0;                        n = 3; break;//3次元
		case ePrm_t::eNoAccel: y1 = 0.333333f;				n = 3; break;//直線の場合は3次元中1/3の点
		case ePrm_t::eRapid_Lv1: y1 = 1;						n = 3; break;//3次元
		case ePrm_t::eRapid_Lv2: y1 = 1;						n = 5; break;//5次元
		case ePrm_t::eRapid_Lv3: y1 = 1;						n = 7; break;//7次元
		case ePrm_t::eRapid_Lv4: y1 = 1;						n = 9; break;//9次元
		case ePrm_t::eRapid_Lv5: y1 = 1;						n = 11; break;//11次元
		}
		switch (ePrm2){
		case ePrm_t::eSlow_Lv5: y2 = 1;                       n = 11; break;//11次元
		case ePrm_t::eSlow_Lv4: y2 = 1;                       n = 9; break;//9次元
		case ePrm_t::eSlow_Lv3: y2 = 1;                       n = 7; break;//7次元
		case ePrm_t::eSlow_Lv2: y2 = 1;                       n = 5; break;//5次元
		case ePrm_t::eSlow_Lv1: y2 = 1;                       n = 3; break;//3次元
		case ePrm_t::eNoAccel: y2 = 0.6666667f;              n = 3; break;//直線の場合は3次元中2/3の点
		case ePrm_t::eRapid_Lv1: y2 = 0;                       n = 3; break;//3次元
		case ePrm_t::eRapid_Lv2: y2 = 0;                       n = 5; break;//5次元
		case ePrm_t::eRapid_Lv3: y2 = 0;                       n = 7; break;//7次元
		case ePrm_t::eRapid_Lv4: y2 = 0;                       n = 9; break;//9次元
		case ePrm_t::eRapid_Lv5: y2 = 0;                       n = 11; break;//11次元
		}

		float	out = Get( y1, y2, fRate, n );
		return out;
	}

//----------------------------------------------------------------------------
//	図形設定
//----------------------------------------------------------------------------

	//	頂点設定
	void	SetVertex( LVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color )
	{
		v.x = x;
		v.y = y;
		v.z = z;
		v.tu = tu;
		v.tv = tv;
		v.color = color;
	}

	//	頂点初期化
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
//	放物線(出力、開始点、目標地点、初速、かかる負荷(重さ))
//----------------------------------------------------------------------
	bool Parabola(Vector3& out, Vector3 start, Vector3 end, float speed, float mass)
	{
		float angleTan;
		float elevation;	//仰角
		float height = start.y - end.y;	//高低差
		float tx = end.x - start.x;	//　X座標差
		float tz = end.z - start.z;	//　Z座標差

		float txz = sqrtf( tx * tx + tz * tz );

		// y = v0 * sinθ*t + 0.5f*G*t*t
		// x = v0 * cosθ*t
		// 2次方程式 a*T*T + b*T + c = 0
		float A = (mass*txz*txz) / (2.0f*speed*speed);

		// Tの係数(aT*T + b*T + c = 0)	
		float a, b, c;
		a = 1.0f;
		if (A != 0){
			b = txz / A;
			c = 1.0f + height / A;
		}
		// 解の公式(平方根)
		float D = b * b - 4 * a * c;
		if (D < 0)
		{
			out = Vector3(0, 0, 0);
			angleTan = 0;
			return false;
		}

		// 解の公式
		elevation = (-b + sqrtf(D)) / (2.0f*a);

		// 仰角
		angleTan = atanf(elevation);

		// 移動距離
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
//	相互変換DirectX<->IEX
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
//	演算系
//-------------------------------------------------------------------------
	
	//　( min <= value && value < max )
	bool isMiddle(const int& value, const int& min, const int& max)
	{
		return	(min <= value && value < max);
	}
	bool isMiddle(const float& value, const float& min, const float& max)
	{
		return	(min <= value && value < max);
	}