#include <iostream>
#include <fstream>
#include <sstream>

#include <jubatus/client/recommender_client.hpp>
#include <jubatus/client/recommender_types.hpp>

using namespace std;

const string HOST = "localhost";
const int    PORT = 9199;
const string NAME = "";
const string ALGORITHM   = "lsh";

int main(int argc, char* argv[]){
    ifstream ifs("config.json");
    stringstream ss;
    ss << ifs.rdbuf();

    jubatus::config_data config;
    config.method = ALGORITHM;
    config.converter = ss.str();

    jubatus::client::recommender r(HOST, PORT, 5);
    r.set_config(NAME, config);

    jubatus::datum d;

    d.num_values.push_back(make_pair("movie_A", 5));
    d.num_values.push_back(make_pair("movie_B", 2));
    d.num_values.push_back(make_pair("movie_C", 3));
    r.update_row(NAME, "user01", d);
    d.num_values.clear();

    d.num_values.push_back(make_pair("movie_A", 2));
    d.num_values.push_back(make_pair("movie_B", 5));
    d.num_values.push_back(make_pair("movie_C", 1));
    r.update_row(NAME, "user02", d);
    d.num_values.clear();

    d.num_values.push_back(make_pair("movie_A", 5));
    d.num_values.push_back(make_pair("movie_B", 1));
    d.num_values.push_back(make_pair("movie_C", 4));
    r.update_row(NAME, "user03", d);
    d.num_values.clear();

    vector<string> rows = r.get_all_rows(NAME);
    for (size_t i = 0; i < rows.size(); i++) {
        string id = rows[i];
        jubatus::similar_result result;
        result = r.similar_row_from_id(NAME, id, 3);
        cout << id << " is similar to: " << endl;
        cout << "  ";
        for (size_t n = 0; n < result.size(); n++) {
            cout << result[n].first << " (" << result[n].second << "), ";
        }
        cout << endl;
    }
}