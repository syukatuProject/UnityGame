
#ifndef	 __GLOBALFUNCTION_H__
#define	 __GLOBALFUNCTION_H__

//******************************************************************************
//
//	�O���[�o���֐�
//
//******************************************************************************

//----------------------------------------------------------------------
//	�萔
//----------------------------------------------------------------------

#define	ZERO_VECTOR3		Vector3( 0.0f, 0.0f, 0.0f )
#define	FOR( minValue, maxValue )	for( int value = minValue; value < maxValue; value++ )
#define	FOR_LIST( minValue, maxValue )	for( auto it = minValue; it != maxValue; it++ )
#define	SUM( value ) ( value / 2 ) * ( value + 1 )

//----------------------------------------------------------------------
//	�\����
//----------------------------------------------------------------------

	//	�}�`�`��p�\����
	struct _VB
	{
		LPDIRECT3DVERTEXBUFFER9	p;
		_VB( LPDIRECT3DDEVICE9 d3dd, UINT length ) : p( 0 ){ d3dd->CreateVertexBuffer( length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL ); }
		virtual	~_VB(void) { if ( p )	p->Release(); }
		operator LPDIRECT3DVERTEXBUFFER9( void ){ return p; }
		LPDIRECT3DVERTEXBUFFER9		operator -> (){ return p; }
	};

//----------------------------------------------------------------------
//	�V�X�e��
//----------------------------------------------------------------------

	//	��� �|�C���^�[��delete����nullptr��������
	template<typename T>
	void	SafeDelete( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete	ptr;
			ptr = nullptr;
		}
	}

	//	�s���� �s��̃|�C���^�[��delete[]����nullptr��������
	template<typename T>
	void	SafeDeleteArray( T*& ptr )
	{
		if ( ptr != nullptr )
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	//	�O�t���[������̌o�ߎ��Ԏ擾
	float GetElapseTime( void );

//----------------------------------------------------------------------
//	�ϊ�
//----------------------------------------------------------------------

	//	int�^���l��POINT�ɕϊ����ϊ����POINT��������
	POINT	GetPoint( int x, int y );

	//	�e�F,�����x��ݒ肵�ϊ����DWORD��������
	DWORD	GetColor( float r, float g, float b, float a );
	DWORD	GetColor( Vector3 color );
	DWORD	GetColor( Vector3 color, float alpha );

	//	���[���h���W����N���C�G���g���W�ւ̕ϊ�
	//	out��pos�̃X�N���[�����W��������
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat );

	//	�N���C�A���g���W���烏�[���h���W�ւ̕ϊ�
	//	�ϊ���̃��[���h���W�A�J�����̉��ւ̌����A�X�N���[�����W
	void	ClientToWorld( Vector3& outPos, Vector3& outVec, POINT screenPos );
	
	//	���C�𓖂Ă郁�b�V���A�o�́A�X�N���[�����W
	void	ClientToWorld( iexMesh* targetMesh, Vector3& out, const POINT& screenPos );

//----------------------------------------------------------------------
//	�}�`�ݒ�
//----------------------------------------------------------------------

	//	���_�ݒ�
	void	SetVertex( LVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

	//	���_�ݒ�
	void	SetVertex( TLVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color );

	//	�n�_�I�_�̓�����
	namespace ePrm_t
	{
		enum {           // Prm1                   / Prm2
			eSlow_Lv5,      // ���@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv4,      // ���@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv3,      // �@�@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv2,      // ���@������蓮���n�߂� / ������蓮���I����
			eSlow_Lv1,      // ��@������蓮���n�߂� / ������蓮���I����
			eNoAccel,       // �@�@�����I�ȓ���������
			eRapid_Lv1,     // ��@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv2,     // ���@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv3,     // �@�@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv4,     // ���@�}�ɓ����n�߂�      / �}�ɓ����I����
			eRapid_Lv5,     // ���@�}�ɓ����n�߂�      / �}�ɓ����I����
		};
	}

	//	�������x�W�F�Ȑ��p�����[�^�擾( 0.0f ~ 1.0f )
	float GetBezier( int ePrm1, int ePrm2, float fRate );

//----------------------------------------------------------------------
//	������(�o�́A�J�n�_�A�ڕW�n�_�A�����A�d��)
//----------------------------------------------------------------------
	bool Parabola(Vector3& out, Vector3 start, Vector3 end, float speed, float mass);

//----------------------------------------------------------------------
//	D3DX��IEX�̓���ւ�
//----------------------------------------------------------------------
	void QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ );
	void QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ );
	void Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV );
	void Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV );

//----------------------------------------------------------------------
//	���Z
//----------------------------------------------------------------------
	inline void Increment(int& out){ out++; }
	bool isMiddle(const int& value, const int& min, const int& max);
	bool isMiddle(const float& value, const float& min, const float& max);

//******************************************************************************
#endif // !__GROBALFUNCTION_H__
