
#pragma once

//**********************************************************************
//
//	PlayerManager�N���X
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
	//	�������E���
	PlayerManager( void );
	~PlayerManager( void );

public:
	//	�������E���
	bool Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
	
	//	����֐�

	//	���ݒ�

	//	���擾
	static	PlayerManager*	GetInstance( void );
};

#define	playerManager ( PlayerManager::GetInstance() )