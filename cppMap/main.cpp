#include <unordered_map>
#include <string>
#include <iostream>
int main(){
	std::unordered_map<std::string,int> tzz;
	tzz["name"] =1;
	tzz["name2"] =2;
	std::unordered_map<std::string,int>::iterator t = tzz.begin();
	int count =0;
	for(std::unordered_map<std::string,int>::iterator t=tzz.begin();t!=tzz.end();++t){
		count++;
		std::cout<<t->first<<std::endl;
		std::cout<<t->second<<std::endl;
		std::cout<<"\n";
	}
	std::cout<<count<<std::endl;
}



