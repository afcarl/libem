#ifndef MATRIX_HEADER_H
#define MATRIX_HEADER_H

#include <string>
#include <exception>
#include <stdexcept>

//! class for returning errors due to mismatched size in matrix operations */
class SizeError: public std::runtime_error
{
	public:
	SizeError(char* error="Matrix operation could not be performed due to a size mismatch."):std::runtime_error(error){}
	
	
};

//! class for returning errors when attempting an operation exclusive to nonsingular matrices on a singular matrix */
/*class SingularError: public std::runtime_error
{
	public:
	SingularError(char* error="Operation could not be performed--perhaps matrix is singular?"):std::runtime_error(error){}
	
};*/

//! Exception thrown when there is an error in a call through lapacke */
class LapackError: public std::runtime_error
{
	public:
	LapackError(char* error="Operation could not be performed--error in call to lapacke"):std::runtime_error(error){}
	
};


class Matrix
{
	public:
	/** enum for specifying how a matrix is represented as an array - used for matrix constructor*/
	enum Orientation{ROW_MAJOR, COLUMN_MAJOR };

	/**create an empty matrix (size 0x0) */
	Matrix();

	/** create a matrix of zeroes, of size nxm 
	@param n number of rows
	@param m number of columns*/
	Matrix(int n, int m);

	/**Constructor to initialize matrix to 0's off-diagonal, and a given vector on the diagonal (a la numpy.diag)
	@param di -- vector of length len, to be used as the diagonal
	param len length of di*/
	Matrix(double di[], int len);

	/**Create a matrix from its column-major or row-major matrix representation
	@param a array of row-major or column-major representation of matrix
	@param nRows current number of rows
	@param nCols current number of columns
	@param major Orientation.ROW_MAJOR or Orientation.COLUMN_MAJOR*/
	Matrix(double a[], int numRows, int numCols, Orientation major);

	/**Assign val to the ith row, jth column of the matrix
	@param val double value to insert
	@param i row number
	@param j column number*/
	void assign(double val, int i, int j);

	/**Overwrite val in the ith row, jth column of the matrix
	@param val double value to write
	@param i row number
	@param j column number*/
	void update(double val, int i, int j);

	/**Overload [] so that i-j th entry can be accessed and assigned by matrix[i, j]
	@param i row number
	@param j column number
	@return reference to value in ith row, jth column*/
	//double & operator()(int i, int j);

	/**Insert row as the rowNum'th row in the matrix (indexed from 0)
	@param row the row to insert 
	@param rowSize the length of row (should be the same as colCount() for non-empty matrix)
	@param rowNum location at which to insert row -- row will be the rowNum'th row 
		in the matrix (indexed from 0) (0 <= rowNum <= rowCount())*/
	Matrix &  insertRow(double * row, int rowSize, int rowNum) throw (SizeError);

	/**Insert col as the colNum'th column in the matrix (indexed from 0)
	@param col the column to insert
	@param colSize the length of col (should be the same as numRows() for non-empty matrix)
	@param colNum location at which to insert col -- col will be the colNum'th column 
		in the matrix (indexed from 0) (0  <= colNum <= colCount())*/
	Matrix &  insertColumn(double * col, int colSize, int colNum) throw (SizeError);
	
	/** How many rows are in the matrix?
	@return the number of rows*/
	int rowCount();
	
	/**  How many columns are in the matrix?
	@return the number of columns*/
	int colCount();

	/**@return the element in ith row, jth column (indexed from 0)*/
	double getValue(int row, int column);

	/**Invert the matrix
	@return the inverse of this matrix*/
	Matrix & inv() throw (SizeError, LapackError);

	/**Matrix multiplication -- this*B
	@param B matrix to multiply by
	@return product of matrix multiplication: this*B  */
	Matrix & dot(Matrix& m);

	/**@return the determinant. Note: only works for square matrices*/
	double det() throw (LapackError, SizeError);

	/** Compute the covariance of a positive, definite matrix (numpy.covar)
	@return the covariance matrix  */
	Matrix & covar();
	
	/**return a copy of rowOffset'th row of the matrix
	@param rowOffset number of the row to retrieve (indexed from 0)
	@return vector representation of the specified row*/
	std::vector<double> getCopyOfRow(int rowOffset) throw (SizeError);

	/** return a copy of colOffset'th row of the matrix
	@param colOffset number of the column to retrieve (indexed from 0)
	@return vector representation of the specified column*/
	std::vector<double> getCopyOfColumn(int colOffset) throw (SizeError);

	/**compute the weighted average value of each row or column
	@param axis row by row (0) or column by column (1)
	@param  weights optional array of weights for weighted average (if NULL, will be taken as all 1's).  
		(should be length numCols() for row-by-row, and length numRows() for col-by-col)*/
	double* average(int axis, double *weights);

	/**  Add vector to matrix row by row or column by column (in place)
	@param vector array to add
	@param m length of vector
	@param axis row by row (0) or column by column (1)*/
	void add(double vector[], int m, int axis);

	/** Subtract Matrix B from this Matrix 
	@param B Matrix to subtract from this*/
	Matrix& subtract(Matrix& B);

	/**  Subtract vector from matrix row by row or column by column (in place)
	@param vector array to subtract
	@param m length of vector
	@param axis row by row (0) or column by column (1)*/
	void subtract(double vector[], int m, int axis);

	/** Print the matrix to stdout */
	void print();
	
	/** clear all entries in the matrix. leaves an empty 0 x 0 matrix. */
	void clear();

	/** Destuctor */
	~Matrix();

	private:
	std::vector< std::vector<double> > columns; ///columns of matrix--this is kept up to date as matrix is modified
	
	/// representation of matrix as 1-d array in column major order.  This is not kept up to date as
	/// matrix is created/modified, but only updated when needed for an operation
	double * entries ; 

	void updateArray(); ///< update column-major representation in entries

	bool changed;//< has matrix been modified since last call to updateArray()?
	int numRows; ///< number of rows in matrix
	int numCols; ///< number of columns in matrix

	/// matrix mult A*B, where A and B are matrices represented in column-major form, with dimensions
	/// rows1xcols1 and cols1xcols2 respectively.
	void dot(double A[], double B[], int rows1, int cols1, int cols2, double result[]);
	
	
};
#endif //MATRIX_HEADER
