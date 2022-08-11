#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>

#include "../Engine/LoadResult.hpp"


namespace egn
{
	class TileSet
	{
	public:
		enum LoadType { Single, Bulk };

		enum SortMethod { None, Alphabetical };

		TileSet()
		{
			// default..
		}

		~TileSet()
		{
			std::cout << "bye bye" << std::endl;
		}

		bool create(const sf::Vector2f& tileSize, sf::Uint32 tileCount)
		{
			tileCount += 1; // for the additional transparent tile

			if (m_tileSet_rendTex.create(tileSize.x * tileCount, tileSize.y))
			{
				return 1;
			}
			m_tileSize = tileSize;

			m_tileSet_rendTex.clear(sf::Color::Transparent);

			return 0;
		}

		sf::Int32 getTileCount()
		{
			return m_tileCount;
		}

		sf::Vector2f getTileSize()
		{
			return m_tileSize;
		}

		const sf::Texture& getTileSet() const
		{
			// type of this is sf::RenderTexture
			return m_tileSet_rendTex.getTexture();
		}

		void loadFromTexture(const sf::Texture& texture)
		{
			sf::Sprite newTile_spr(texture);
			newTile_spr.setPosition(this->m_tileCount++ * this->m_tileSize.x, 0);
			this->m_tileSet_rendTex.draw(newTile_spr);
			this->m_tileSet_rendTex.display();
		}

		bool loadFromFile(std::string path, LoadType loadType = LoadType::Single)
		{
			sf::Texture texture;
			if (!texture.loadFromFile(path)) {
				return LoadResult::Failure;
			}
			sf::Sprite newTile_spr(texture);
			newTile_spr.setPosition(this->m_tileCount++ * this->m_tileSize.x, 0);
			this->m_tileSet_rendTex.draw(newTile_spr);
			this->m_tileSet_rendTex.display();

			return LoadResult::Success;
		}

		bool loadFromDirectory(std::string path, SortMethod sortMethod = SortMethod::Alphabetical)
		{
			std::vector<std::filesystem::path> filesInDirectory;
			std::copy(std::filesystem::directory_iterator(path),
				std::filesystem::directory_iterator(), std::back_inserter(filesInDirectory));
			std::sort(filesInDirectory.begin(), filesInDirectory.end());


			if (sortMethod == SortMethod::Alphabetical) {
				std::sort(filesInDirectory.begin(), filesInDirectory.end());
			}

			for (auto& file : filesInDirectory) {
				std::string ext = file.extension().string();

				if (ext == ".png" || ext == ".jpg") // feel free to add more ext
				{
					this->loadFromFile(file.string(), LoadType::Single);
				}
			}
			return LoadResult::Success;
		}

		void clear()
		{
			m_tileCount = 0;

			m_tileSize = sf::Vector2f(80, 80);

			m_tileSet_rendTex.clear(sf::Color::Transparent);
		}

	private:
		sf::Int32 m_tileCount = 0;

		sf::Vector2f m_tileSize = sf::Vector2f(80, 80);

		sf::RenderTexture m_tileSet_rendTex;
	};
}