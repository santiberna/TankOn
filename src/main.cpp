#include <SDL3/SDL_main.h>
#include <utility/imgui_common.hpp>
#include <engine/audio.hpp>
#include <game/application.hpp>

// void server()
// {
//     using namespace asio::ip;

//     try
//     {
//         boost::asio::io_context io_context;
//         udp::socket socket(io_context, udp::endpoint(udp::v4(), 6000));

//         for (;;)
//         {
//             std::array<char, 1> recv_buf;
//             udp::endpoint remote_endpoint {};
//             socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

//             std::string message = "Hello World!";
//             boost::system::error_code ignored_error;
//             socket.send_to(boost::asio::buffer(message),
//                 remote_endpoint, 0, ignored_error);
//         }
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
// }

// void client()
// {
//     using namespace asio::ip;

//     try
//     {
//         asio::io_context io_context;
//         asio::ip::udp::resolver resolver(io_context);
//         asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "6000").begin();

//         udp::socket socket(io_context);
//         socket.open(udp::v4());

//         std::array<char, 1> send_buf = { { 0 } };
//         socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

//         std::array<char, 128> recv_buf;
//         udp::endpoint sender_endpoint;
//         size_t len = socket.receive_from(
//             boost::asio::buffer(recv_buf), sender_endpoint);

//         std::cout.write(recv_buf.data(), len);
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << e.what() << std::endl;
//     }
// }

int main(int, char*[])
{
    SDLAbortIfFailed(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO));
    SDLAbortIfFailed(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG));
    SDLAbortIfFailed(Mix_OpenAudio(0, nullptr));

    {
        Application game {};
        imgui_shortcuts::InitSDL3(game.renderer.GetWindow(), game.renderer.GetRenderer());

        // Music music = Music::Create("assets/music/Retro_Platforming-David_Fesliyan.mp3").value();
        // music.Start();

        while (!game.input.ShouldClose())
        {
            imgui_shortcuts::StartFrame();

            game.DoFrame();

            imgui_shortcuts::EndFrame(game.renderer.GetRenderer());
            SDL_RenderPresent(game.renderer.GetRenderer());
        }

        imgui_shortcuts::ShutdownSDL3();
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    return 0;
}