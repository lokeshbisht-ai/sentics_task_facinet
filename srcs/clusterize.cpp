#include "../headers/clusterize.hpp"

void	clusterize(const std::string & dummy_data, const std::string & clusterized_data)
{
	std::vector<t_entry> 				data = get_data(dummy_data);
	std::vector<f_entry>	clusters; 

	std::map<std::string, std::vector<t_entry> > sensor_groups = group_by_sensors(data);
	std::map<std::string, std::vector<t_entry> >::iterator group;
	for (group = sensor_groups.begin(); group != sensor_groups.end(); ++group)
	{
		std::map<std::string, std::vector<t_entry> > id_groups = group_by_uniq_id(group->second);
		std::map<std::string, std::vector<t_entry> >::iterator ids;
		for (ids = id_groups.begin(); ids != id_groups.end(); ++ids)
		{
			std::vector<std::vector<t_entry> > dist_groups = group_by_distance(ids->second);
			std::vector<std::vector<t_entry> >::iterator it_dist_g;
			for (it_dist_g = dist_groups.begin(); it_dist_g != dist_groups.end(); ++it_dist_g)
			{
                // Compute the average timestamp
                double avg_timestamp = 0.0;
                for (std::vector<t_entry>::iterator it = it_dist_g->begin(); it != it_dist_g->end(); ++it)
				{
                    avg_timestamp += std::strtod(it->timestamp_id.c_str(), new char*);
                }
                avg_timestamp /= it_dist_g->size();

                // Assign a random f_id
                int f_id = rand();

                // Assign f_u_id to the first object that has a unique_id
                int f_u_id = 0;
                for (std::vector<t_entry>::iterator it = it_dist_g->begin(); it != it_dist_g->end(); ++it)
				{
                    if (std::atoi(it->unique_id.c_str()) != 0) {
                        f_u_id = std::atoi(it->unique_id.c_str());
                        break;
                    }
                }

                // Add the cluster to the list of clusters
				f_entry cluster;
				cluster.f_id = f_id;
				cluster.cluster_data.sensor_id = it_dist_g->front().sensor_id;
				cluster.cluster_data.x_position= it_dist_g->front().x_position;
				cluster.cluster_data.y_position= it_dist_g->front().y_position;
				cluster.f_u_id = f_u_id;
				cluster.f_timestamp = avg_timestamp;
                clusters.push_back(cluster);

                // Assign f_u_id to the remaining objects without a unique_id
				for (std::vector<t_entry>::iterator it = it_dist_g->begin(); it != it_dist_g->end(); ++it)
				{
                    if (std::atoi(it->unique_id.c_str()) == 0) {
                        it->unique_id = f_u_id;
                    }
                }
			}
		}
	}
	(void) clusterized_data;
	// !! TODO : create a csv file name clusterized_data, iteratate clusters and print row in file.
	generate_fusioned_data(clusters, clusterized_data);
}

std::vector<t_entry>	get_data(const std::string & dummy_data)
{
	std::ifstream				csv_file(dummy_data.c_str());
	t_entry						entry;
	std::vector<std::string>	columns;
	std::vector<t_entry>		data;

	if (csv_file.is_open())
	{
		std::string line;
		getline(csv_file, line);
		while(getline(csv_file, line))
		{
			columns = split_entry(line, ',');
			if (columns.size() != NB_COLUMNS)
			{
				std::cout << "Error : Dummy data not well formatted\n";
				exit(0);
			}
			entry.timestamp_id = columns[0];
			entry.sensor_id = columns[1];
			entry.id = columns[2];
			entry.x_position = std::atoi(columns[3].c_str());
			entry.y_position = std::atoi(columns[4].c_str());
			entry.unique_id = columns[5];
			data.push_back(entry);
		}
	}
	else
	{
		std::cout << "Error: Could not open dummy data\n";
	}
	return data;
}

std::vector<std::string> split_entry(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

bool	compare_by_timestamp(const t_entry & it_1, const t_entry & it_2)
{
	return it_1.timestamp_id < it_2.timestamp_id;
}

std::map<std::string, std::vector<t_entry> >	group_by_sensors(const std::vector<t_entry> & data)
{
	std::map<std::string, std::vector<t_entry> > sensor_groups;
	int											left = 0, right = data.size();
	while (left < right)
	{
		sensor_groups[data[left].sensor_id].push_back(data[left]);
		sensor_groups[data[right - 1].sensor_id].push_back(data[right - 1]);
		++left;
		--right;
	}
	return sensor_groups;
}

std::map<std::string, std::vector<t_entry> >	group_by_uniq_id(const std::vector<t_entry> & data)
{
	std::map<std::string, std::vector<t_entry> > id_groups;
	int											left = 0, right = data.size();
	while (left < right)
	{
		id_groups[data[left].unique_id].push_back(data[left]);
		id_groups[data[right - 1].unique_id].push_back(data[right - 1]);
		++left;
		--right;
	}
	return id_groups;
}

std::vector<std::vector<t_entry> > group_by_distance(const std::vector<t_entry> & data)
{
	std::vector<std::vector<t_entry> > dist_groups;
	for (size_t i = 0; i < data.size(); ++i) {
		bool added = false;
		for (size_t j = 0; j < dist_groups.size(); ++j) {
			if (distance(data[i], dist_groups[j].front()) <= 2.0) {
				dist_groups[j].push_back(data[i]);
				added = true;
				break;
			}
		}
		if (!added) {
			std::vector<t_entry> tmp;
			tmp.push_back(data[i]);
			dist_groups.push_back(tmp);
		}
	}
	return dist_groups;
}


double distance(const t_entry & it_1, const t_entry & it_2) {
    double dx = it_1.x_position - it_2.x_position;
    double dy = it_1.y_position - it_2.y_position;
    return std::sqrt(dx * dx + dy * dy);
}

void											generate_fusioned_data( const std::vector<f_entry> & clusters,\
														 				const std::string & clusterized_data )
{
	FILE*	tmp;
	tmp = fopen(clusterized_data.c_str(), "w");
	fclose(tmp);
	std::ofstream file(clusterized_data);
	if (file.is_open())
	{
		file << "f_id,cluster_data,f_timestamp,f_u_id\n";
		std::vector<f_entry>::const_iterator it;
		
		for (it = clusters.begin(); it != clusters.end(); ++it)
		{
			file << it->f_id;
			file << ",";
			file << it->cluster_data.sensor_id;
			file << " ";
			file << it->cluster_data.x_position;
			file << " ";
			file << it->cluster_data.y_position;
			file << ",";
			file << it->f_timestamp;
			file << ",";
			file << it->f_u_id;
			file << "\n";
		}
		file.close();
	}
	else
	{
		std::cout << "ERROR: Could not open the clustrized_data\n";
	}

}