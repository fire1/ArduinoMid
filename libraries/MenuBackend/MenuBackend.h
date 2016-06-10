/*
||
|| @file MenuBackend.h
|| @version 1.1
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making menus
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef MenuBackend_h
#define MenuBackend_h

class MenuBackend;
class MenuItem
{
	friend class MenuBackend; //provide acces to private 
public:

	MenuItem(const char* itemName, const char itemShortkey = 0) : name(itemName), shortkey(itemShortkey)
	{
		before = 0;
		right = 0;
		after = 0;
		left = 0;
		back = 0;
	}

	//void use(){} //update some internal data / statistics

	const char* getName() const { return name; }
	bool hasShortkey() const { return (shortkey!=0); }
	const char getShortkey() const { return shortkey; }
	
	
	MenuItem *getBack() { return back; }
	
	MenuItem *getBefore() { return before; }
	MenuItem *getRight() { return right; }
	MenuItem *getAfter() { return after; }
	MenuItem *getLeft() { return left; }

	MenuItem *moveUp() { 
		if (before)
		{	
			before->back = this;
		}
		return before; 
	}

	MenuItem *moveDown() { 
		if (after)
		{ 
			after->back = this;
		}
		return after; 
	}

	MenuItem *moveLeft() { 
		if (left)
		{
			left->back = this;
		}
		return left; 
	}

	MenuItem *moveRight() { 
		if (right)
		{	
			right->back = this;
		}
		return right; 
	}
	//default vertical menu
	MenuItem &add(MenuItem &mi) { return addAfter(mi); }

	MenuItem &addBefore(MenuItem &mi)
	{
		mi.set(this,AFTER);
		set(&mi,BEFORE);
		return mi;
	}
	MenuItem &addRight(MenuItem &mi)
	{
		mi.set(this,LEFT);
		set(&mi,RIGHT);
		return mi;
	}
	MenuItem &addAfter(MenuItem &mi)
	{
		mi.set(this,BEFORE);
		set(&mi,AFTER);
		return mi;
	}
	MenuItem &addLeft(MenuItem &mi)
	{
		mi.set(this,RIGHT);
		set(&mi,LEFT);
		return mi;
	}
protected:
	typedef const unsigned char insert_t;
	static insert_t BEFORE = 0; 
	static insert_t RIGHT = 1; 
	static insert_t AFTER = 2; 
	static insert_t LEFT = 3; 
	void set(MenuItem *mi, insert_t at)
	{
		switch (at)
		{
		case BEFORE: before = mi; break;
		case RIGHT: right = mi; break;
		case AFTER: after = mi; break;
		case LEFT: left = mi; break;
		}
	}
	
	const char* name;
	const char shortkey;

	MenuItem *before;
	MenuItem *right;
	MenuItem *after;
	MenuItem *left;
	MenuItem *back;
};


//no dependant inclusion of string or cstring
bool menuTestStrings(const char *a, const char *b)
{
	while (*a) { if (*a != *b) { return false; } b++; a++; }
	return true;
}
bool operator==(MenuItem &lhs, char* test)
{
	return menuTestStrings(lhs.getName(),test);
}
bool operator==(const MenuItem &lhs, char* test)
{
	return menuTestStrings(lhs.getName(),test);
}
bool operator==(MenuItem &lhs, MenuItem &rhs)
{
	return menuTestStrings(lhs.getName(),rhs.getName());
}
bool operator==(const MenuItem &lhs, MenuItem &rhs)
{
	return menuTestStrings(lhs.getName(),rhs.getName());
}

struct MenuChangeEvent
{
	const MenuItem &from;
	const MenuItem &to;
};

struct MenuUseEvent
{
	const MenuItem &item;
};

typedef void (*cb_change)(MenuChangeEvent);
typedef void (*cb_use)(MenuUseEvent);

class MenuBackend
{
public:
	MenuBackend(cb_use menuUse, cb_change menuChange = 0) : root("MenuRoot")
	{
		current = &root;
		cb_menuChange = menuChange;
		cb_menuUse = menuUse;
	}

    void toRoot() {
        setCurrent( &getRoot() );
    }

	MenuItem &getRoot()
	{
		return root;
	}
	MenuItem &getCurrent()
	{
		return *current;
	}

	void moveBack()
	{
		setCurrent(current->getBack());
	}

	void moveUp()
	{
		setCurrent(current->moveUp());
	}

	void moveDown()
	{
		setCurrent(current->moveDown());
	}

	void moveLeft()
	{
		setCurrent(current->moveLeft());
	}

	void moveRight()
	{
		setCurrent(current->moveRight());
	}

	void use(char shortkey)
	{
		recursiveSearch(shortkey,&root);
		use();
	}

	void use()
	{
		//current->use();
		if (cb_menuUse) 
		{
			MenuUseEvent mue = { *current };
			(*cb_menuUse)(mue);
		}
	}
private:
	void setCurrent( MenuItem *next )
	{
		if (next)
		{
			MenuChangeEvent mce = { *current, *next };
			current = next;
			if (cb_menuChange) 
			{
				(*cb_menuChange)(mce);
			}
		}
	}
	void foundShortkeyItem(MenuItem *mi)
	{
		mi->back = current;
		current = mi;
	}
	bool canSearch(const char shortkey, MenuItem *m)
	{
		if (m==0) { return false; }
		else 
		{
			if (m->getShortkey()==shortkey)
			{
				foundShortkeyItem(m);
			}
			return true;
		}
	}
	void recursiveSearchAfter(const char shortkey, MenuItem *m)
	{
		if (canSearch(shortkey,m))
		{
			recursiveSearchAfter(shortkey, m->after);
			recursiveSearchRight(shortkey, m->right);
			recursiveSearchLeft(shortkey, m->left);
		}
	}
	void recursiveSearchRight(const char shortkey, MenuItem *m)
	{
		if (canSearch(shortkey,m))
		{
			recursiveSearchAfter(shortkey, m->after);
			recursiveSearchRight(shortkey, m->right);
			recursiveSearchBefore(shortkey, m->before);
		}
	}
	void recursiveSearchLeft(const char shortkey, MenuItem *m)
	{
		if (canSearch(shortkey,m))
		{
			recursiveSearchAfter(shortkey, m->after);
			recursiveSearchLeft(shortkey, m->left);
			recursiveSearchBefore(shortkey, m->before);
		}
	}
	void recursiveSearchBefore(const char shortkey, MenuItem *m)
	{
		if (canSearch(shortkey,m))
		{
			recursiveSearchRight(shortkey, m->right);
			recursiveSearchLeft(shortkey, m->left);
			recursiveSearchBefore(shortkey, m->before);
		}
	}
	void recursiveSearch(const char shortkey, MenuItem *m)
	{
		if (canSearch(shortkey,m))
		{
			recursiveSearchAfter(shortkey, m->after);
			recursiveSearchRight(shortkey, m->right);
			recursiveSearchLeft(shortkey, m->left);
			recursiveSearchBefore(shortkey, m->before);
		}
	}
	MenuItem root;
	MenuItem *current;
	cb_change cb_menuChange;
	cb_use cb_menuUse;
};

#endif
