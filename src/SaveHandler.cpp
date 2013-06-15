/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <QDataStream>
#include <QFile>
#include "SaveHandler.h"
#include "../DiaballikEngine/src/BitContainerInputStream.h"
#include "gameConstants.h"

SaveHandler::SaveHandler (const QString& filename) {
	this->filename = filename;
	this->figures = vector<Point>();
	this->player = -1;
	this->history = vector<vector<Move> >();
}

bool SaveHandler::load() {
	//open QDataStream
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly) == false)
		return false;
	
	QDataStream in(&file);
	
	//read DIABSAVE
	QString diabsave = "";
	quint8 tmp;
	while (diabsave.length() < 8 && !in.atEnd()) {
		in >> tmp;
		diabsave.append(tmp);
	}
	
	if (diabsave != "DIABSAVE" || in.atEnd())
		return false;	//incorrect beginning
	
	int bytes = 12;
	//read 12 first bytes
	vector<uint8_t> data(bytes, 0);
	int i;
	for (i = 0; i < bytes && !in.atEnd(); ++i)
		in >> data[i];
	
	if (i < bytes || in.atEnd())
		return false;	//file too short
		
	BitContainerInputStream b(data);
	b.setBitsPerValue(3);
	
	this->figures.clear();
	this->player = -1;
	this->history.clear();
	//parse 'em
	
	for (i = 0; i < 16; ++i)	//2 x 7 pawns + 2 x 1 ball
		this->figures.push_back(Point(b.getNextValue(), b.getNextValue()));
	
	//history - continuous read/parse in order to avoid errors by
	//file corruption
	//read player number first
	b = BitContainerInputStream();
	in >> tmp;
	b.addBits(tmp);
	b.setBitsPerValue(1);
	this->player = b.getNextValue();
	
	int coordId, pointId;
	
	uint8_t coord[2];
	Point move[2];
	vector<Move> turn;
	
	b.setBitsPerValue(3);
	//read the rest:
	
	coordId = pointId = 0;
	while (b.hasNext() && this->history.size() <= SANE_HISTORY_SIZE) {
		coord[coordId] = b.getNextValue();	//get next coordinate
		
		if (coord[coordId] == 7) {	//if it's an end of a turn
			if (coordId != 0 || pointId != 0)	//end of turn mid-point/move
				return false;		//it means the file is corrupted
			
			//add the turn
			this->history.push_back(turn);
			turn.clear();
		} else {
			if (coordId == 1) {	//if we have the whole point
				move[pointId] = Point(coord[0], coord[1]);
				if (pointId == 1) {	//if we have the whole move
					if (move[0] == move[1])
						return true;	//might be the end of the file, but
						//we don't care. A move to self means that the rest of the history
						//is invalid
					turn.push_back(Move(move[0], move[1]));
					pointId = 0;
				} else pointId++;
				coordId = 0;
			} else coordId++;
		}
		
		if (!b.hasNext() && !in.atEnd()) {
			in >> tmp;
			b.addBits(tmp);
		}
	}
	return true;
}

bool SaveHandler::save (const vector< Point >& figures, const int id, const vector< vector< Move > >& history) {

}

const vector< Point >& SaveHandler::getFigures() const {
	return this->figures;
}

const int SaveHandler::getPlayer() const {
	return this->player;
}

const vector< vector< Move > >& SaveHandler::getHistory() const {
	return this->history;
}



