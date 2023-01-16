from pytube import YouTube

def download_video(link):
  # Create a YouTube object using the link provided
  yt = YouTube(link)

  # Get the highest quality video available
  video = yt.streams.get_highest_resolution()

  # Download the video
  video.download()

  # Print a message to confirm that the download has completed
  print("Download completed!")

while True:
  # Prompt the user to enter a YouTube link
  link = input("Enter a YouTube link (or 'none' to exit): ")

  # Check if the user has entered "none"
  if link.lower() == "none":
    # If so, exit the loop
    break

  # Download the video
  download_video(link)

print("Exiting program...")
