
#pragma once

//**********************************************************************
//
//	Playerクラス
//
//**********************************************************************

//	include
#include	"BaseObj.h"

//	class
class Player : public BaseObj
{
private:
public:
	//	初期化・解放
	Player( void );
	~Player( void );
	bool	Initialize( void )override;

	//	更新・描画
	void	Update( void )override;
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;
	
	//	情報取得
};