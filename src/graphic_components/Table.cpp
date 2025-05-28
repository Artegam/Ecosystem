#include "GraphicComponents.h"

using namespace GraphicComponents;


Table::Table (const int window, const int x, const int y, const unsigned int rows, const unsigned int cols, string defaultval): Leaf(window, x, y) {
  unsigned int nbCells = rows * cols;
  _t.rows = rows;
  _t.cols = cols;
  _width = cols + 1;
  _height = rows;

  cells.clear();
  for (unsigned int i = 0; i < nbCells; i++) {
    Cell * c = new Cell(window, x+_x, y+_y, defaultval);
    cells.push_back(c);
  }
  updateLengths();
}

void Table::updateLengths () {
  unsigned int nbCells = _t.rows * _t.cols;
  for (unsigned int i = 0; i < nbCells; i++) {
    Cell c = getCell(i);
    if (i > 0)
      _width += c.value().length();

    unsigned int col = i%_t.cols;
    if(_t.colssizes.size() < _t.cols)
      _t.colssizes.push_back(c.value().length());
    else {
      list<unsigned int>::iterator it;
      it = _t.colssizes.begin();
      advance(it, col);
      if(*it<(unsigned int)c.value().length())
        *it =c.value().length();
    }

    unsigned int row = i/_t.cols;
    Utils u;

    if(_t.rowssizes.size()<_t.rows)
      _t.rowssizes.push_back(u.occurences(c.value(), '\n')+1);
    else {
      list<unsigned int>::iterator it;
      it = _t.rowssizes.begin();
      advance(it, row);
      if(*it<(unsigned int)u.occurences(c.value(), '\n')+1)
        *it = u.occurences(c.value(), '\n')+1;
    }
  }
}

list<Cell*> Table::getCells () {
  return cells;
}

// Les lignes sont numerotees a partir de zero
list<Cell*> Table::getRow (unsigned int n) {
  list<Cell*> row;
  list<Cell*>::iterator it = cells.begin();
  unsigned int offset = n*_t.cols;
  advance(it, offset);

  for(unsigned int i=0; i<_t.cols; i++) {
    row.push_back(*it);
    it++;
  }

  return row;
}

Cell Table::getCell (const unsigned int pos) {
  list<Cell*>::iterator it;
  it = cells.begin();
  if(pos < cells.size())
    advance(it, pos);
  return **it;
}

Cell * Table::getCell(unsigned int row, unsigned int col) {
  unsigned int position = row*_t.cols+col-1;
  list<Cell*>::iterator it = cells.begin();
  advance(it, position);
  return *it;
}

const tab Table::getTab() {
  return _t;
}

void Table::setValue(unsigned int row, unsigned int col, string value) {
/*
  unsigned int position = row*_t.cols+col-1;
  list<Cell*>::iterator it = cells.begin();
  advance(it, position);
  (*it)->setValue(value);
*/
  Cell * c = getCell(row, col);
  c->setValue(value);
  updateLengths();
}

void Table::setValue(unsigned int row, unsigned int col, const char * value) {
  string val = value;
  setValue(row, col, val);
}

void Table::select(unsigned int row, unsigned int col) {
  Cell * c = getCell(row, col);
  c->select();
}
