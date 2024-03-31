export module file_util;
import <filesystem>;
import <random>;
import <string>;
import <fstream>;

namespace fs = std::filesystem;
using std::string;

export struct TemporaryFile {
    fs::path inner_path;

    public :
    TemporaryFile() {
        this->inner_path = fs::temp_directory_path();

        static std::mt19937_64 generator;
        std::uniform_int_distribution<uint64_t> distribution(0, std::numeric_limits<uint64_t>::max());
        const uint64_t random_number = distribution(generator);

        this->inner_path = this->inner_path / (string{"dev.dagans.ricky."} + std::to_string(random_number));
    }

    const fs::path& path() const {
        return this->inner_path;
    }

    ~TemporaryFile() {
        fs::remove(this->inner_path);
    }
};

export namespace FileUtil {
    std::string get_contents(const fs::path& path) {
        std::ifstream fp(path.native());
        std::string contents;
        std::stringstream buffer;
        if (fp.fail()) {
            throw std::runtime_error(string{"Could not open file: "} + path.native());
        }
        buffer << fp.rdbuf();
        contents = buffer.str();
        fp.close();
        return contents;
    }
}
