from conan import ConanFile


class MCSConan(ConanFile):
    name = "MCS-Algo_Test"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "gtest/1.13.0",
    ]
    generators = "CMakeDeps", "CMakeToolchain"

    def configure(self):
        self.options["gtest"].shared = False
