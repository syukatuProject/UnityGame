
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"BaseObj.h"

//**********************************************************************
//
//	BaseObjクラス
//
//**********************************************************************

//-------------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------------

	//	コンストラクタ
	BaseObj::BaseObj( void ) : obj( nullptr ),
		pos( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f )
	{
	
	}

	//	デストラクタ
	BaseObj::~BaseObj( void )
	{
		SafeDelete( obj );
	}
	
	//	初期化
	bool	BaseObj::Initialize( void )
	{
		//	読み込み

		//	情報設定

		//	情報更新
		UpdateInfo();

		return	false;
	}

	//	解放

	//	読み込み
	bool	BaseObj::Load( const LPSTR filename )
	{
		obj = nullptr;
		if ( obj == nullptr )	obj = new iex3DObj( filename );

		if ( obj != nullptr ) 	return	true;
		return	false;
	}

//-------------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------------

	//	更新
	void	BaseObj::Update( void )
	{
		UpdateInfo();
	}

	//	情報更新
	void	BaseObj::UpdateInfo( void )
	{
		if ( obj == nullptr )	return;

		obj->Animation();
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();
	}

	//	描画
	void	BaseObj::Render( iexShader* shader, LPSTR technique )
	{
		if ( obj == nullptr )	return;

		if ( shader == nullptr && technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			obj->Render( shader, technique );
		}
	}

//-------------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//	情報設定
//-------------------------------------------------------------------------------------

	//	座標設定
	void	BaseObj::SetPos( const Vector3& Pos )
	{
		pos = Pos;
	}

	//	向き設定
	void	BaseObj::SetAngle( float Angle )
	{
		angle = Angle;
	}

	//	スケール設定
	void	BaseObj::SetScale( float Scale )
	{
		scale = Scale;
	}

	//	モーション設定
	void	BaseObj::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}

//-------------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------------

	//	座標取得
	Vector3	BaseObj::GetPos( void )const
	{
		return	pos;
	}

	//	向き取得
	float			BaseObj::GetAngle( void )const
	{
		return	angle;
	}
