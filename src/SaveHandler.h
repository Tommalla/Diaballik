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
		QString filename;
		vector< Point > figures;
		int player;
		vector< vector< Move > > history;
	public:
		SaveHandler(const QString& filename);
		
		bool load();
 		bool save(const vector<Point>& figures, const int id, const vector<vector<Move> >& history); //TODO
		
		const vector<Point>& getFigures() const;
		const int getPlayer() const;
		const vector<vector<Move> >& getHistory() const;
};

#endif // SAVEHANDLER_H
