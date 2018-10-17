#include "BloomEffect.h"

BloomEffect::BloomEffect()
    : m_shaders()
    , m_brightnessTexture()
    , m_firstPassTextures()
    , m_secondPassTextures()
{
    m_shaders.load (Shaders::BrightnessPass,
                    "media/Shaders/Fullpass.vert", "media/Shaders/Brightness.frag");
    m_shaders.load (Shaders::DownSamplePass,
                    "media/Shaders/Fullpass.vert", "media/Shaders/DownSample.frag");
    m_shaders.load (Shaders::GaussianBlurPass,
                    "media/Shaders/Fullpass.vert", "media/Shaders/GuassianBlur.frag");
    m_shaders.load (Shaders::AddPass,
                    "media/Shaders/Fullpass.vert", "media/Shaders/Add.frag");
}

void BloomEffect::apply (const sf::RenderTexture& input,
                         sf::RenderTarget& output)
{
    prepareTextures (input.getSize());

    filterBright (input, m_brightnessTexture);

    downsample (m_brightnessTexture, m_firstPassTextures[0]);
    blurMultipass (m_firstPassTextures);

    downsample (m_firstPassTextures[0], m_secondPassTextures[0]);
    blurMultipass (m_secondPassTextures);

    add (m_firstPassTextures[0], m_secondPassTextures[0],
         m_firstPassTextures[1]);
    m_firstPassTextures[1].display();

    add (input, m_firstPassTextures[1], output);
}

void BloomEffect::prepareTextures (sf::Vector2u size)
{
    if (m_brightnessTexture.getSize() != size)
    {
        m_brightnessTexture.create (size.x, size.y);
        m_brightnessTexture.setSmooth (true);

        m_firstPassTextures[0].create (size.x / 2, size.y / 2);
        m_firstPassTextures[0].setSmooth (true);
        m_firstPassTextures[1].create (size.x / 2, size.y / 2);
        m_firstPassTextures[1].setSmooth (true);

        m_secondPassTextures[0].create (size.x / 4, size.y / 4);
        m_secondPassTextures[0].setSmooth (true);
        m_secondPassTextures[1].create (size.x / 4, size.y / 4);
        m_secondPassTextures[1].setSmooth (true);
    }
}

void BloomEffect::filterBright (const sf::RenderTexture& input,
                                sf::RenderTexture& output)
{
    sf::Shader& brightness = m_shaders.get (Shaders::BrightnessPass);

    brightness.setParameter ("source", input.getTexture());
    applyShader (brightness, output);
    output.display();
}

void BloomEffect::blurMultipass (RenderTextureArray& renderTextures)
{
    sf::Vector2u textureSize = renderTextures[0].getSize();

    for (std::size_t count = 0; count < 2; ++count)
    {
        blur (renderTextures[0], renderTextures[1], sf::Vector2f (0.f,
                1.f / textureSize.y));
        blur (renderTextures[1], renderTextures[0],
              sf::Vector2f (1.f / textureSize.x, 0.f));
    }
}

void BloomEffect::blur (const sf::RenderTexture& input,
                        sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
    sf::Shader& gaussianBlur = m_shaders.get (Shaders::GaussianBlurPass);

    gaussianBlur.setParameter ("source", input.getTexture());
    gaussianBlur.setParameter ("offsetFactor", offsetFactor);
    applyShader (gaussianBlur, output);
    output.display();
}

void BloomEffect::downsample (const sf::RenderTexture& input,
                              sf::RenderTexture& output)
{
    sf::Shader& downSampler = m_shaders.get (Shaders::DownSamplePass);

    downSampler.setParameter ("source", input.getTexture());
    downSampler.setParameter ("sourceSize",
                              sf::Vector2f (input.getSize()));
    applyShader (downSampler, output);
    output.display();
}

void BloomEffect::add (const sf::RenderTexture& source,
                       const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
    sf::Shader& adder = m_shaders.get (Shaders::AddPass);

    adder.setParameter ("source", source.getTexture());
    adder.setParameter ("bloom", bloom.getTexture());
    applyShader (adder, output);
}












