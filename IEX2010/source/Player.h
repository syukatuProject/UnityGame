
#pragma once

//**********************************************************************
//
//	Player�N���X
//
//**********************************************************************

//	include
#include	"BaseObj.h"

//	class
class Player : public BaseObj
{
private:
public:
	//	�������E���
	Player( void );
	~Player( void );
	bool	Initialize( void )override;

	//	�X�V�E�`��
	void	Update( void )override;
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;
	
	//	���擾
};