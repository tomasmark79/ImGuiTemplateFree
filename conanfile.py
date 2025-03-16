import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.system import package_manager
from conan.tools.files import copy

# In Pro Template DO NOT use cmake_layout(self) in the recipe.
# It is self implemented in SolutionController.py

class DotNameCppRecipe(ConanFile):
    
    name = "imguilib"
    version = "1.0"
    
    # Settings
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    
    # Binary configuration
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    
    # ----------------------------------------------------------
    # Consuming recipe
    # ----------------------------------------------------------
    
    def configure(self):
        self.options["*"].shared = False # this replaced shared flag from SolutionController.py and works
        self.options["sdl"].iconv = False # iconv not supported by mingw-w64
        
    def requirements(self):
        self.requires("fmt/[~11.1]") # required by cpm package
        # self.requires("zlib/[~1.3]")
        # self.requires("nlohmann_json/[~3.11]")
        # self.requires("yaml-cpp/0.8.0")
        self.requires("sdl/2.30.9")
        self.requires("glfw/3.4")
        self.requires("glew/2.2.0")
        self.requires("imgui/1.91.5")


    def generate(self):
        copy(self, "*glfw*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "src/bindings"))
        copy(self, "*opengl3*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "src/bindings"))
        copy(self, "*sdl2*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "src/bindings"))
        
    def build_requirements(self):
        self.tool_requires("cmake/[>3.14]")

    # def system_requirements(self):
        # dnf = package_manager.Dnf(self)
        # dnf.install("SDL2-devel")
        # apt = package_manager.Apt(self)
        # apt.install(["libsdl2-dev"])
        # yum = package_manager.Yum(self)
        # yum.install("SDL2-devel")
        # brew = package_manager.Brew(self)
        # brew.install("sdl2")

    # TO DO 
    # # ----------------------------------------------------------    
    # # Creating basic library recipe
    # # ----------------------------------------------------------
    
    # # Optional metadata
    # license = "<Put the package license here>"
    # author = "<Put your name here> <And your email here>"
    # url = "<Package recipe repository url here, for issues about the package>"
    # description = "<Description of hello package here>"
    # topics = ("<Put some tag here>", "<here>", "<and here>")
   
    # # Sources are located in the same place as this recipe, copy them to the recipe
    # exports_sources = "CMakeLists.txt", "src/*", "include/*", "cmake/*", "LICENSE", "README.md"
    
    # def config_options(self):
    #     if self.settings.os == "Windows":
    #         del self.options.fPIC

    # def build(self):
    #     cmake = CMake(self)
    #     cmake.configure()
    #     cmake.build()

    # def package(self):
    #     cmake = CMake(self)
    #     cmake.install()

    # def package_info(self):
    #     self.cpp_info.libs = ["imguilib"]
