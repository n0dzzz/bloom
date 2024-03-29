#pragma once

template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
    int* pVTable = *(int**)ppClass;
    int dwAddress = pVTable[index];
    return (FuncType)(dwAddress);
}

class IPanel
{
public:
	
    void SetKeyboardInputEnabled(unsigned int panel_id, bool state)
    {
        using original_fn = void(__thiscall*)(IPanel*, unsigned int, bool);
        return (*(original_fn**)this)[31](this, panel_id, state);
    }

    void SetMouseInputEnabled(unsigned int panel_id, bool state)
    {
        using original_fn = void(__thiscall*)(IPanel*, unsigned int, bool);
        return (*(original_fn**)this)[32](this, panel_id, state);
    }
	
    const char* GetName(unsigned int vguiPanel)
    {
        typedef const char* (__thiscall* tGetName)(void*, unsigned int);
        return CallVFunction<tGetName>(this, 36)(this, vguiPanel);
    }
#ifdef GetClassName
#undef GetClassName
#endif
    const char* GetClassName(unsigned int vguiPanel)
    {
        typedef const char* (__thiscall* tGetClassName)(void*, unsigned int);
        return CallVFunction<tGetClassName>(this, 37)(this, vguiPanel);
    }
};