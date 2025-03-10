#pragma once

#include <string>
#include <vector>

#include <agui/agui.h>
#include <agui/utilities/fwd-agui.h>

using std::string;
using std::vector;

/**
 * String tokenizer class
 * @author Andreas Drewke
 */
class agui::utilities::StringTokenizer
{

private:
	vector<string> tokens;
	int idx { 0 };

public:
	/**
	 * Public constructor
	 */
	StringTokenizer();

	/**
	 * Tokenize
	 * @param str string to tokenize
	 * @param delimiters delimiters
	 * @param emptyTokens include empty tokens
	 */
	void tokenize(const string& str, const string& delimiters, bool emptyTokens = false);

	/**
	 * @returns number of tokens
	 */
	inline int32_t countTokens() {
		return tokens.size();
	}

	/**
	 * @returns has more tokens
	 */
	inline bool hasMoreTokens() {
		return idx != tokens.size();
	}

	/**
	 * @returns next token
	 */
	inline const string& nextToken() {
		return tokens[idx++];
	}

	/**
	 * @returns tokens
	 */
	inline const vector<string>& getTokens() {
		return tokens;
	}

};
