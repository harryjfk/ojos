// ---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H

class TLista {
private:
	int r[100];

	int c;

public:
	TLista() {
		c = 0;
	}

	TLista(int n) {
		c = n;
	}

	void Add(int n) {
		r[c] = n;
		c++;
	}

	int Count() {
		return c;
	}

	void Delete(int i) ;

	int Item(int pos) {
		return r[pos];
	}

	void Item(int pos, int value) {
		r[pos] = value;
	}

	int IndexOf(int n) {
	}

	void Clear() {
	}

	int mayornumero() {
		int m = 0;
		for (int i = 0; i < c; i++)
			if (m < Item(i)) {
				m = Item(i);

			}
		return m;
	}

};

#endif
