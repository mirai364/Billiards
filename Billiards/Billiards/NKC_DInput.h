//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
/* キーボード用 */
extern BYTE					g_diKeyState[256];
/* マウス用 */
extern DIMOUSESTATE			g_diMouseState;

//-----------------------------------------------------------------------------
// プロトタイプ宣言 function-prototypes
//-----------------------------------------------------------------------------
/* DirectInput共通 */
bool InitDInput(HINSTANCE);
void ReleaseDInput(void);
/* キーボード用 */
bool InitKeyboard(void);
void GetKeyboardState(void);
/* マウス用 */
bool InitMouse(void);
void GetMouseState(void);

//-----------------------------------------------------------------------------
// グローバル変数（他のソースからも利用されるもの）
//-----------------------------------------------------------------------------
/* キーボード用 */
BYTE					g_diKeyState[256];			// キーボード情報
/* マウス用 */
DIMOUSESTATE			g_diMouseState;				// マウス情報

//-----------------------------------------------------------------------------
// グローバル変数（自ソースでのみ利用するもの）
//-----------------------------------------------------------------------------
/* DirectInput共通 */
static LPDIRECTINPUT8		g_lpDInput = NULL;			// DirectInputオブジェクト
/* キーボード用 */
static LPDIRECTINPUTDEVICE8	g_lpDIKeyboard = NULL;		// キーボードデバイス
/* マウス用 */
static LPDIRECTINPUTDEVICE8	g_lpDIMouse = NULL;			// マウスデバイス