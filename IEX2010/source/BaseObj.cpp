
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"BaseObj.h"

//**********************************************************************
//
//	BaseObj�N���X
//
//**********************************************************************

//-------------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	BaseObj::BaseObj( void ) : obj( nullptr ),
		pos( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f )
	{
	
	}

	//	�f�X�g���N�^
	BaseObj::~BaseObj( void )
	{
		SafeDelete( obj );
	}
	
	//	������
	bool	BaseObj::Initialize( void )
	{
		//	�ǂݍ���

		//	���ݒ�

		//	���X�V
		UpdateInfo();

		return	false;
	}

	//	���

	//	�ǂݍ���
	bool	BaseObj::Load( const LPSTR filename )
	{
		obj = nullptr;
		if ( obj == nullptr )	obj = new iex3DObj( filename );

		if ( obj != nullptr ) 	return	true;
		return	false;
	}

//-------------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------------

	//	�X�V
	void	BaseObj::Update( void )
	{
		UpdateInfo();
	}

	//	���X�V
	void	BaseObj::UpdateInfo( void )
	{
		if ( obj == nullptr )	return;

		obj->Animation();
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();
	}

	//	�`��
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
//	����֐�
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------------

	//	���W�ݒ�
	void	BaseObj::SetPos( const Vector3& Pos )
	{
		pos = Pos;
	}

	//	�����ݒ�
	void	BaseObj::SetAngle( float Angle )
	{
		angle = Angle;
	}

	//	�X�P�[���ݒ�
	void	BaseObj::SetScale( float Scale )
	{
		scale = Scale;
	}

	//	���[�V�����ݒ�
	void	BaseObj::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}

//-------------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------------

	//	���W�擾
	Vector3	BaseObj::GetPos( void )const
	{
		return	pos;
	}

	//	�����擾
	float			BaseObj::GetAngle( void )const
	{
		return	angle;
	}
