#ifndef CLUSTERIZE_HPP
# define CLUSTERIZE_HPP
# include <iostream>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>
# include <algorithm>
# include <map>
# include <cmath>
# include <cstdio>

# define  NB_COLUMNS 6

typedef struct s_entry {
	std::string timestamp_id;
	std::string sensor_id;
	std::string id;
	int 		x_position;
	int			y_position;
	std::string	unique_id;

}				t_entry;

typedef struct s_clust_d {
	int		  	x_position;
	int		  	y_position;
	std::string	sensor_id;
}				clust_d;

typedef struct s_f_entry {
	int						f_id;
	clust_d					cluster_data;
	int						f_u_id;
	double					f_timestamp;
}				f_entry;

void											clusterize(const std::string & dummy_data, const std::string & clusterized_data);
std::vector<t_entry>							get_data(const std::string & dummy_data);
std::vector<std::string>						split_entry(const std::string& s, char delim);
bool											compare_by_timestamp(const t_entry & it_1, const t_entry & it_2);
std::map<std::string, std::vector<t_entry> >	group_by_sensors(const std::vector<t_entry> & data);
std::map<std::string, std::vector<t_entry> >	group_by_uniq_id(const std::vector<t_entry> & data);
std::vector<std::vector<t_entry> > 				group_by_distance(const std::vector<t_entry> & data);
double 											distance(const t_entry & it_1, const t_entry & it_2);
void											generate_fusioned_data(const std::vector<f_entry> & clusters, const std::string & clusterized_data);
#endif
