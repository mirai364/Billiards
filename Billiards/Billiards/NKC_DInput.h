//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
/* �L�[�{�[�h�p */
extern BYTE					g_diKeyState[256];
/* �}�E�X�p */
extern DIMOUSESTATE			g_diMouseState;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾 function-prototypes
//-----------------------------------------------------------------------------
/* DirectInput���� */
bool InitDInput(HINSTANCE);
void ReleaseDInput(void);
/* �L�[�{�[�h�p */
bool InitKeyboard(void);
void GetKeyboardState(void);
/* �}�E�X�p */
bool InitMouse(void);
void GetMouseState(void);

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��i���̃\�[�X��������p�������́j
//-----------------------------------------------------------------------------
/* �L�[�{�[�h�p */
BYTE					g_diKeyState[256];			// �L�[�{�[�h���
/* �}�E�X�p */
DIMOUSESTATE			g_diMouseState;				// �}�E�X���

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��i���\�[�X�ł̂ݗ��p������́j
//-----------------------------------------------------------------------------
/* DirectInput���� */
static LPDIRECTINPUT8		g_lpDInput = NULL;			// DirectInput�I�u�W�F�N�g
/* �L�[�{�[�h�p */
static LPDIRECTINPUTDEVICE8	g_lpDIKeyboard = NULL;		// �L�[�{�[�h�f�o�C�X
/* �}�E�X�p */
static LPDIRECTINPUTDEVICE8	g_lpDIMouse = NULL;			// �}�E�X�f�o�C�X