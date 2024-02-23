#ifndef Cube_h
#define Cube_h
#include "Side.h"
#include "Row.h"

using namespace std;

/* Keep a virtual cube in the program to keep track of the permutations being applied */

class Cube {
private:
	/* Virtual cube contains a side for each side of the actual cube (6 total) */
	bool solved;

	void createEmpty(); // initializes an empty cube

public:

	Side up, right, left, front, back, down;
	
	Cube();
	Cube(const Cube &c);
	Side getSide(int a) const;
	void moveSide(int motion);
	
	bool operator==(const Cube& other) const {
		return up.getupper().getright() == other.up.getupper().getright()
    		&& up.getupper().getleft() == other.up.getupper().getleft()
    		&& up.getlower().getright() == other.up.getlower().getright()
    		&& up.getlower().getleft() == other.up.getlower().getleft()
    		&&
    		down.getupper().getright() == other.down.getupper().getright()
    		&& down.getupper().getleft() == other.down.getupper().getleft()
    		&& down.getlower().getright() == other.down.getlower().getright()
    		&& down.getlower().getleft() == other.down.getlower().getleft()
    		&&
    		right.getupper().getright() == other.right.getupper().getright()
    		&& right.getupper().getleft() == other.right.getupper().getleft()
    		&& right.getlower().getright() == other.right.getlower().getright()
    		&& right.getlower().getleft() == other.right.getlower().getleft()
    		&&
    		left.getupper().getright() == other.left.getupper().getright()
    		&& left.getupper().getleft() == other.left.getupper().getleft()
    		&& left.getlower().getright() == other.left.getlower().getright()
    		&& left.getlower().getleft() == other.left.getlower().getleft()
    		&&
    		front.getupper().getright() == other.front.getupper().getright()
    		&& front.getupper().getleft() == other.front.getupper().getleft()
    		&& front.getlower().getright() == other.front.getlower().getright()
    		&& front.getlower().getleft() == other.front.getlower().getleft()
    		&&
    		back.getupper().getright() == other.back.getupper().getright()
    		&& back.getupper().getleft() == other.back.getupper().getleft()
    		&& back.getlower().getright() == other.back.getlower().getright()
    		&& back.getlower().getleft() == other.back.getlower().getleft();
	}

	/* Input/Output */
	void set(int side, int a, int b, int c, int d);
	void displayCube();

	/* Validation */
	bool isValid();
	bool isSolved();

	/* Permutations */
	void U(); // U clockwise
	void Up(); // U counterclockwise
	void U2();

	void R();
	void Rp();
	void R2();
	
	void L();
	void Lp();
	void L2();
	
	void F();
	void Fp();
	void F2();

	void B();
	void Bp();
	void B2();

	void D();
	void Dp();
	void D2();
};

/* Algorithm notation: 
 * U, R, L, F, B, D corresponds to the up, right, left, front, back, and down faces.
 * The default is clockwise; ' (prime) denotes counterclockwise.
 */

/* Color notation:
 * 0: white
 * 1: red
 * 2: blue
 * 3: orange
 * 4: green
 * 5: yellow
 */
#endif
