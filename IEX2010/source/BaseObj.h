
#pragma once

//**********************************************************************
//
//	BaseObj�N���X
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
	//	�������E���
	BaseObj( void );
	virtual ~BaseObj( void );
	virtual bool Initialize( void );
	bool	Load( const LPSTR filename );

	//	�X�V�E�`��
	virtual void	Update( void );
	void	UpdateInfo( void );
	virtual void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	����֐�

	//	���ݒ�
	void	SetPos( const Vector3& Pos );
	void	SetAngle( float Angle );
	void	SetScale( float scale );
	void	SetMotion( int motion );

	//	���擾
	Vector3	GetPos( void )const;
	float			GetAngle( void )const;
};
