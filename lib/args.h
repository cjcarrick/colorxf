#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * Parses argv, assuming it follows the form:
 *
 * --key value
 * --flag1 --flag2
 * -k value
 *
 * In order to interpret
 *     -k --key2 positional
 * correctly, with `positional` as a positional and not a value for --key2, call
 * .set_bool_opt("key2").
 */
class Args
{

private:
    vector<string> positionals;
    vector<string> keys;
    vector<string> values;

public:
    Args(char **argv, int argc);

    void set_bool_opt(const string &key);

    /**
     * Retrieve the value for a specified key, and store it in value.
     *
     * Returns true if the key exists, and false otherwise.
     * 
     * String value is only modified if a value is actually found.
     */
    bool get(const string &key, string &value) const;
    bool get(char key, string &value) const;

    /**
     * Returns true if the key exists, and false otherwise
     */
    bool get(const string &key) const;
    bool get(char key) const;

    /**
     * Get a vector of all keys
     */
    vector<string> get() const;

    /**
     * Get a vector of all positionals.
     */
    vector<string> get_positionals() const;
};

void print_help();
