/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#ifndef SAVEHANDLER_H
#define SAVEHANDLER_H

#include <QString>
#include <vector>
#include "../DiaballikEngine/src/Point.h"
#include "../DiaballikEngine/src/Move.h"

using namespace std;

class SaveHandler {
	private:
		vector< vector< Point > > pawns;
		vector< Point > balls;
		int player;
		vector< vector< Move > > history;
	public:
		SaveHandler(const QString& filename);
		
		bool load();
// 		bool save() //TODO
};

#endif // SAVEHANDLER_H
