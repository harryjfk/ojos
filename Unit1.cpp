// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void S(int* a, float* w) {
	*a = 5;
	*w = 7.8;
}

void s1(int a, float w) {
	a = 5;
	w = 7.8;
}

String Sarcasmo(String A, int D) {
	bool in = false;
	String t = A;
	for (int i = 1; i <= D; i++) {
		for (int j = 1; j <= t.Length(); j++) {
			if ( t.Length()-D == j-i ) {
				in = true;
				for (int k =  j-1; k <= t.Length(); k++) {
					if (t[k] == '1')
						t[k] = '0';
					else
						t[k] = '1';
				}

			}
			else if (t[j] == '0')
				{}
			else {
				t[j] = '0';
				i++;  break;
			}

			if (in)
				break;
		}
		if (in)
			break;
	}
	return t;
}

// void W(void**
void __fastcall TForm1::Button1Click(TObject *Sender) {
	/* int* a=new int;
	 float *w = new float;
	 ShowMessage(IntToStr(*a));
	 S(a,w);
	 ShowMessage(IntToStr(*a));

	 TA* r  =new TA();
	 r->AAA(); */
	/* list->Add(random(900));
	 ListBox1->Clear();
	 for (int i = 0; i < list->Count(); i++) {
	 ListBox1->Items->Add(IntToStr(list->Item(i)));
	 } */
	ShowMessage(Sarcasmo("10111", 2));
}

void TA::AAA() {
	ShowMessage("a");
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender) {

	// list =new TLista(4);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender) {
	ShowMessage(IntToStr(list->mayornumero()));
}
// ---------------------------------------------------------------------------
