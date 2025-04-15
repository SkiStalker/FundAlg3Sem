#include <iostream>
#include <string>
#include <vector>

class LoggerBuilder;

class Logger {
   private:
	friend class LoggerBuilder;
	std::vector<std::reference_wrapper<std::ostream>> handlers;

	Logger() {}

	unsigned int log_level;

   public:
	void error(const std::string& str) {
        for (auto& iter: this->handlers) {
            iter.get() << "<date>:" << "ERROR:" << str << std::endl; 
        }
    }
};

class LoggerBuilder {
   private:
	Logger* logger = nullptr;

   public:
	LoggerBuilder() { this->logger = new Logger(); }

	LoggerBuilder& set_level(unsigned int logging_level) {
		this->logger->log_level = logging_level;
		return *this;
	}

	LoggerBuilder& add_handler(std::ostream& stream) {
		this->logger->handlers.push_back(stream);
		return *this;
	}

	Logger* make_object() { return this->logger; }
};



int main() {
	Logger* my_logger = LoggerBuilder()
    .set_level(1)
    .add_handler(std::cout)
    .make_object();


    my_logger->error("PAINC");

    delete my_logger;
	return 0;
}