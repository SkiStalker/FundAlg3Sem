#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class LoggerBuilder;

class Logger {
   private:
	friend class LoggerBuilder;
	std::vector<std::ostream*> handlers;
	std::vector<std::unique_ptr<std::ostream>> own_handlers;

	Logger() {}

	unsigned int log_level;

   public:
	void error(const std::string& str) {
		for (auto&& iter : this->handlers) {
			(*iter) << "<date>:" << "ERROR:" << str << std::endl;
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
		this->logger->handlers.push_back(&stream);
		return *this;
	}

	LoggerBuilder& add_handler(std::unique_ptr<std::ostream> stream) {
		this->logger->handlers.push_back(stream.get());
		this->logger->own_handlers.push_back(std::move(stream));
		return *this;
	}

	Logger* make_object() { return this->logger; }
};

int main() {
	Logger* my_logger = LoggerBuilder()
	                        .set_level(1)
	                        .add_handler(std::cout)
	                        .add_handler(std::make_unique<std::ofstream>("test.txt"))
	                        .make_object();

	my_logger->error("PAINC");

	delete my_logger;
	return 0;
}