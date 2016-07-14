
#pragma once

//**********************************************************************
//
//	BaseObjクラス
//
//**********************************************************************

//	include

//	class
class BaseObj
{
private:
	iex3DObj*	obj;
	Vector3		pos;
	float				angle;
	float				scale;

public:
	//	初期化・解放
	BaseObj( void );
	virtual ~BaseObj( void );
	virtual bool Initialize( void );
	bool	Load( const LPSTR filename );

	//	更新・描画
	virtual void	Update( void );
	void	UpdateInfo( void );
	virtual void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数

	//	情報設定
	void	SetPos( const Vector3& Pos );
	void	SetAngle( float Angle );
	void	SetScale( float scale );
	void	SetMotion( int motion );

	//	情報取得
	Vector3	GetPos( void )const;
	float			GetAngle( void )const;
};
