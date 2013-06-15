/* Tomasz [Tommalla] Zakrzewski, 2013
All rights reserved */

#include <QDataStream>
#include <QFile>
#include "SaveHandler.h"
#include "../DiaballikEngine/src/BitContainerInputStream.h"
#include "../DiaballikEngine/src/BitContainerOutputStream.h"
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
	
	qDebug("Started reading %s", filename.toStdString().c_str());
	
	//read DIABSAVE
	QString diabsave = "";
	quint8 tmp;
	while (diabsave.length() < 8 && !in.atEnd()) {
		in >> tmp;
		diabsave.append(tmp);
	}
	
	if (diabsave != "DIABSAVE" || in.atEnd())
		return false;	//incorrect beginning
		
	qDebug(diabsave.toStdString().c_str());
	
	int bytes = 12;
	//read 12 first bytes
	vector<uint8_t> data(bytes, 0);
	int i;
	for (i = 0; i < bytes && !in.atEnd(); ++i)
		in >> data[i];
	
	if (i < bytes || in.atEnd())
		return false;	//file too short
		
	for (uint8_t row: data)
		qDebug("%d", row);
		
	BitContainerInputStream b(data);
	b.setBitsPerValue(3);
	
	this->figures.clear();
	this->player = -1;
	this->history.clear();
	//parse 'em
	
	for (i = 0; i < 16; ++i) {	//2 x 7 pawns + 2 x 1 ball
		int x, y;
		x = b.getNextValue();
		y = b.getNextValue();
		this->figures.push_back(Point(x, y));
		qDebug("read pawn at %d %d", this->figures.back().x, this->figures.back().y);
	}
	
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
		
		if (turn.size() > 3)
			return false;
	}
	return true;
}

bool SaveHandler::save (const vector< Point >& figures, const int id, const vector< vector< Move > >& history) {
	//open QDataStream
	QFile file(filename);
	if (file.open(QIODevice::WriteOnly) == false)
		return false;
	
	QDataStream out(&file);
	
	qDebug("Started writing to %s", filename.toStdString().c_str());
	qDebug("Sizes: %d, %d\n", figures.size(), history.size());
	
	//write out DIABSAVE
	QString diabsave = "DIABSAVE";
	for (int i = 0; i < 8; ++i)
		out << (quint8)diabsave[i].toAscii();
	
	BitContainerOutputStream b;
	b.setBitsPerValue(3);
	for (Point point: figures) {
		b.append(point.x);
		b.append(point.y);
	}
	
	b.setBitsPerValue(1);
	b.append(id);
	b.setBitsPerValue(3);
	for (auto iter = history.begin(); iter != history.end(); ++iter) {
		if (iter != history.begin())
			b.append(7);	//end of turn
		for (Move move: *iter) {
			b.append(move.from.x);
			b.append(move.from.y);
			b.append(move.to.x);
			b.append(move.to.y);
		}
	}
	
	vector<quint8> data = b.getData();
	for (quint8 row: data)
		out << (unsigned char)row;
	
	for (quint8 row: data)
		qDebug("%d", row);
	
	return true;
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



