
#ifndef	 __GLOBALFUNCTION_H__
#define	 __GLOBALFUNCTION_H__

//******************************************************************************
//
//	グローバル関数
//
//******************************************************************************

//----------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------

#define	ZERO_VECTOR3		Vector3( 0.0f, 0.0f, 0.0f )
#define	FOR( minValue, maxValue )	for( int value = minValue; value < maxValue; value++ )
#define	FOR_LIST( minValue, maxValue )	for( auto it = minValue; it != maxValue; it++ )
#define	SUM( value ) ( value / 2 ) * ( value + 1 )

//----------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------

	//	図形描画用構造体
	struct _VB
	{
		LPDIRECT3DVERTEXBUFFER9	p;
		_VB( LPDIRECT3DDEVICE9 d3dd, UINT length ) : p( 0 ){ d3dd->CreateVertexBuffer( length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL ); }
		virtual	~_VB(void) { if ( p )	p->Release(); }
		operator LPDIRECT3DVERTEXBUFFER9( void ){ return p; }
		LPDIRECT3DVERTEXBUFFER9		operator -> (){ return p; }
	};

//----------------------------------------------------------------------
//	システム
//----------------------------------------------------------------------

	//	解放 ポインターをdeleteしてnullptrを代入する
	template<typename T>
	void	SafeDelete( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete	ptr;
			ptr = nullptr;
		}
	}

	//	行列解放 行列のポインターをdelete[]してnullptrを代入する
	template<typename T>
	void	SafeDeleteArray( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	//	前フレームからの経過時間取得
	float GetElapseTime( void );

//----------------------------------------------------------------------
//	変換
//----------------------------------------------------------------------

	//	int型数値をPOINTに変換し変換後のPOINTをかえす
	POINT	GetPoint( int x, int y );

	//	各色,透明度を設定し変換後のDWORDをかえす
	DWORD	GetColor( float r, float g, float b, float a );
	DWORD	GetColor( Vector3 color );
	DWORD	GetColor( Vector3 color, float alpha );

	//	ワールド座標からクライエント座標への変換
	//	outにposのスクリーン座標をかえす
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat );

	//	クライアント座標からワールド座標への変換
	//	変換後のワールド座標、カメラの奥への向き、スクリーン座標
	void	ClientToWorld( Vector3& outPos, Vector3& outVec, POINT screenPos );
	
	//	レイを当てるメッシュ、出力、スクリーン座標
	void	ClientToWorld( iexMesh* targetMesh, Vector3& out, const POINT& screenPos );

//----------------------------------------------------------------------
//	図形設定
//----------------------------------------------------------------------

	//	頂点設定
	void	SetVertex( LVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

	//	頂点設定
	void	SetVertex( TLVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

	//	始点終点の動作種類
	namespace ePrm_t
	{
		enum {           // Prm1                   / Prm2
			eSlow_Lv5,      // 強　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv4,      // ↑　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv3,      // 　　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv2,      // ↓　ゆっくり動き始める / ゆっくり動き終える
			eSlow_Lv1,      // 弱　ゆっくり動き始める / ゆっくり動き終える
			eNoAccel,       // 　　直線的な動きをする
			eRapid_Lv1,     // 弱　急に動き始める      / 急に動き終える
			eRapid_Lv2,     // ↑　急に動き始める      / 急に動き終える
			eRapid_Lv3,     // 　　急に動き始める      / 急に動き終える
			eRapid_Lv4,     // ↓　急に動き始める      / 急に動き終える
			eRapid_Lv5,     // 強　急に動き始める      / 急に動き終える
		};
	}

	//	高次元ベジェ曲線パラメータ取得( 0.0f ~ 1.0f )
	float GetBezier( int ePrm1, int ePrm2, float fRate );

//----------------------------------------------------------------------
//	放物線(出力、開始点、目標地点、初速、重力)
//----------------------------------------------------------------------
	bool Parabola(Vector3& out, Vector3 start, Vector3 end, float speed, float mass);

//----------------------------------------------------------------------
//	D3DXとIEXの入れ替え
//----------------------------------------------------------------------
	void QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ );
	void QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ );
	void Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV );
	void Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV );

//----------------------------------------------------------------------
//	演算
//----------------------------------------------------------------------
	inline void Increment(int& out){ out++; }
	bool isMiddle(const int& value, const int& min, const int& max);
	bool isMiddle(const float& value, const float& min, const float& max);

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
