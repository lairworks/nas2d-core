// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Common.h"
#include "File.h"

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

namespace NAS2D {

/**
 * \class Filesystem
 * \brief Implements a virtual file system.
 *
 * Provides cross-platform and transparent archive Filesystem functions.
 */
class Filesystem
{
public:
    Filesystem() = default;
	~Filesystem();
    Filesystem(const Filesystem&) = delete;
    Filesystem& operator= (const Filesystem&) = delete;
    Filesystem(Filesystem&&) = delete;
    Filesystem& operator=(Filesystem&&) = delete;

	void init(const std::string& argv_0, const std::string& appName, const std::string& organizationName, const std::string& dataPath);

	std::string dataPath() const;
	std::string workingPath(const std::string& filename) const;
	StringList searchPath() const;
	bool mount(const std::string& path) const;

	StringList directoryList(const std::string& dir, const std::string& filter = "") const;

	File read(const std::string& filename) const;
	bool write(const File& file, bool overwrite = true) const;
	bool del(const std::string& path) const;
	bool exists(const std::string& filename) const;

	std::string extension(const std::string& path);

	bool isDirectory(const std::string& path) const;
	bool makeDirectory(const std::string& path) const;

	void toggleVerbose() const;

    void setWorkingDirectory(const std::filesystem::path& p);
    std::filesystem::path getWorkingDirectory() const;

    //Gets the path to the execuable. Returns an empty path if there's an error.
    std::filesystem::path getExePath() const;

private:
    void setWorkingDirectory(const std::filesystem::path& p) const;

    //OS-specific delegate. Do not call this directly. Use getExePath instead.
    std::filesystem::path DoWindowsQueryExePath() const;
    //OS-specific delegate. Do not call this directly. Use getExePath instead.
    std::filesystem::path DoAppleQueryExePath() const;
    //OS-specific delegate. Do not call this directly. Use getExePath instead.
    std::filesystem::path DoLinuxQueryExePath() const;

    bool isInit() const;
    bool exists(const std::filesystem::path& p) const;

    void forEachFileInFolder(const std::filesystem::path& folderpath, const std::string& validExtensionList = std::string{}, const std::function<void(const std::filesystem::path&)>& callback = [](const std::filesystem::path& p) { (void)p; }, bool recursive = false) const;
    void forEachFileInFolder(const std::filesystem::path& folderpath, const std::string& validExtensionList = std::string{}, const std::function<void(const std::filesystem::path&)>& callback = [](const std::filesystem::path& p) { (void)p; }, bool recursive = false);

    template<typename DirectoryIteratorType>
    void forEachFileInFolder(const std::filesystem::path& folderpath, const std::string& validExtensionList = std::string{}, const std::function<void(const std::filesystem::path&)>& callback = [](const std::filesystem::path& p) { (void)p; }, bool recursive = false) const;

    bool readBufferFromFile(std::vector<unsigned char>& out_buffer, const std::string& filePath) const;
    bool readBufferFromFile(std::string& out_buffer, const std::string& filePath) const;

    bool writeBufferToFile(void* buffer, std::size_t size, const std::string& filePath) const;
    bool writeBufferToFile(const std::string& buffer, const std::string& filePath) const;


    mutable std::vector<std::filesystem::path> mSearchPath{};
    std::filesystem::path              mDataPath{};            /**< Data path string. This will typically be 'data/'. */
    std::string                        mOrganizationName{};    /**< The organization name. Only used for compatibility while transitioning from PhysFS */
    std::string                        mAppName{};             /**< The application name. Only used for compatibility while transitioning from PhysFS */
    mutable std::filesystem::path              mWorkingDirectory{};  /**< The working directory. Typically 'mDataPath/mOrganizationName/mAppName/' */
    mutable std::filesystem::path              mExePath{};           /**< Path to the executable.*/
    mutable bool                       mVerbose{ false };      /**< Displays lots of messages when true. Otherwise only critical messages are displayed. */
    mutable bool                       mIsInit{ false };       /**< Has the file system been initialized? */
};

template<typename DirectoryIteratorType>
void forEachFileInFolders(const std::filesystem::path& preferred_folderpath, const std::vector<std::string>& validExtensions, const std::function<void(const std::filesystem::path&)>& callback) {
    if(validExtensions.empty()) {
        std::for_each(DirectoryIteratorType{ preferred_folderpath }, DirectoryIteratorType{},
            [&callback](const std::filesystem::directory_entry& entry) {
            const auto& cur_path = entry.path();
            bool is_directory = std::filesystem::is_directory(cur_path);
            if(!is_directory) {
                callback(cur_path);
            }
        });
        return;
    }
    std::for_each(DirectoryIteratorType{ preferred_folderpath }, DirectoryIteratorType{},
        [&validExtensions, &callback](const std::filesystem::directory_entry& entry) {
        const auto& cur_path = entry.path();
        bool is_directory = std::filesystem::is_directory(cur_path);
        std::string my_extension = toLowercase(cur_path.extension().string());
        bool valid_file_by_extension = std::find(std::begin(validExtensions), std::end(validExtensions), my_extension) != std::end(validExtensions);
        if(!is_directory) {
            if(valid_file_by_extension) {
                callback(cur_path);
            }
        }
    });
}

};
