
#pragma once

//**********************************************************************
//
//	PlayerManagerクラス
//
//**********************************************************************

//	include
#include	"Player.h"

//	class
class PlayerManager
{
private:
	Player*	player;

private:
	//	初期化・解放
	PlayerManager( void );
	~PlayerManager( void );

public:
	//	初期化・解放
	bool Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	
	//	動作関数

	//	情報設定

	//	情報取得
	static	PlayerManager*	GetInstance( void );
};

#define	playerManager ( PlayerManager::GetInstance() )