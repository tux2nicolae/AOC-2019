#pragma once

/**
 * Advent of code 2019
 * @author : Nicolae Telechi
 */
class FStreamReader
{
public:
  /**
   * Input stream
   */
  FStreamReader(ifstream & aIn);

  vector<int> ReadVector();
  vector<int> ReadVectorSeparatedByChar();
  vector<int> ReadLineAsVectorOfDigits();
  vector<string> ReadVectorOfWords();
  vector<string> ReadLineAsVectorOfWords();

  vector<vector<int>> ReadMatrix();
  vector<vector<int>> ReadMatrixOfDigits();
  vector<vector<string>> ReadMatrixOfWords();

private:
  ifstream & mFileStream;

};
